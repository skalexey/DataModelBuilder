import QtQuick
import DMB 1.0
import DataModelBuilderUI 1.0

Window {
    id: root
    width: Constants.width
    height: Constants.height
    visible: true
    // To be used in libraryTypeListModel.onRowsRemoved
    property var rowsRemovedF: uiRoot.onObjectRemoved

	DMBModel {
		id: dmbModel
		onInstantiateRequested: function(instId, protoId) {
			uiRoot.preInstantiateDialog.visible = true
			uiRoot.preInstantiateDialog.initialText = instId
			uiRoot.preInstantiateDialog.onConfirm = function(enteredText) {
				console.log("Instantiate '" + enteredText + "' of type '" + protoId + "'")
				dmbModel.addToContent(enteredText, protoId)
			}
		}
		onInstantiateRefused: function(error) {
			console.log("Instantiate refused: " + error);
		}
		onModelLoaded: function(f) {
			uiRoot.onModelLoaded(f);
		}
		onModelLoadError: function(e) {
			uiRoot.onModelLoadError(e);
		}
	}

	DMBModel {
		id: dmbModelRecentFiles
		onModelLoaded: function (f) {
			uiRoot.onRecentFilesModelLoaded(f);
		}
		onModelLoadError: function(e) {
			uiRoot.onRecentFilesModelLoadError(e);
		}
	}

    DataModelBuilderUI {
        id: uiRoot
        width: root.width
        height: root.height
		// ======= Menu properties ========
		property var currentlyOpenedFile: "";
		openFileClicked: function() {
			chooseFileDialog.visible = true;
		}

		saveAsClicked: function() {
			uiRoot.saveAsDialog.visible = true
			uiRoot.saveAsDialog.initialText = ""
			uiRoot.saveAsDialog.onConfirm = function(enteredText) {
				console.log("Save current model to a file '" + enteredText + "'")
				if (dmbModel.store(enteredText))
					console.log("Stored successfully");
				else
					console.log("Error while storing");
			}
		}

		saveClicked: function() {
			dmbModel.store();
		}

		onOpenFile: function(fPath) {
			dmbModel.load(fPath);
		}


		// Reset default models
		libraryTypeListModel: null
		contentListModel: null
		selectedItemOwnParamListModel: null
		selectedItemProtoParamListModel: null
		selectedItemArrayListModel: null
		recentFilesModel: null

		// ======= Properties for Relative files =======
		function getRecentFileInfo(fPath) {
			return dmbModelRecentFiles.contentModel.get("info").get(fPath);
		}

		recentFileRelPath: function(fPath) {
			var info = getRecentFileInfo(fPath);
			if (info)
				return info.get("relPath").value;
			else
			{
				console.log("Can't find recent file info for '" + fPath + "'");
				return fPath;
			}
		}

		// ======= Properties for Type library functional =======
		function getCurrentObj() {
			//console.log("libraryTypeListModel.object(" + libraryCurrentObjectIndex + ")");
			var obj = dmbModel.typesModel.at(libraryCurrentObjectIndex);
			if (!obj)
				console.log("NULL in getCurrentObj()!");
			return obj;
		}

		// When an object is removed, selectedObject should get the new data
		function setSelectedObjectBindings() {
			libraryTypeListModel = Qt.binding(function() {
				return dmbModel.typesModel.propListModel;
			});
//			property var onObjectRemoved: function() {
//				setSelectedObjectBindings()
//			}
			// TODO: restore
			//dmbModel.typeListModel.onRowsRemoved: function () { rowsRemovedF() }
			libraryAddNewObjectClicked = function() {
				libraryTypeListModel.add("New object", ObjectProperty.Object);
			}
			libraryDeleteObjectClicked = libraryTypeListModel.removeAt
			storeClicked = function () {
				dmbModel.store("model.json");
			}
			instantiateClick = dmbModel.instantiateRequest
			selectedObjectName = Qt.binding(function() {
				var obj = getCurrentObj();
				return obj ? obj.name : "";
			});
			propTypeModel = vlTypeModel;
			selectedObjectNewParamClicked = function() {
				var obj = getCurrentObj();
				if (obj)
					obj.add("New prop", ObjectProperty.String);
			}
			selectedObjectDeleteParameterClicked = function(i) {
				var obj = getCurrentObj();
				if (obj)
					obj.removeAt(i);
			}
			selectedObjParamListModel = Qt.binding(function() {
				var obj = getCurrentObj();
				if (obj)
					return obj.value.propListModel;
				else
					return new ListModel;
			});
		}
		// ======= End of properties for Type library functional =======
		// ======= Properties for Selected content item =======
		function getCurrentItem() {
			//console.log("contentListModel.object(" + contentCurrentItemIndex + ")");
			var item = dmbModel.contentModel.at(contentCurrentItemIndex);
			if (!item)
				console.log("NULL in getCurrentObj()!");
			return item;
		}
		// When an object is removed, selectedObject should get the new data
		function setSelectedItemBindings() {
			selectedItemName = Qt.binding(function() {
				var item = getCurrentItem();
				return item ? item.name : "";
			});
			selectedItemOwnParamListModel = Qt.binding(function() {
				var item = getCurrentItem();
				if (item && item.type === ObjectProperty.Object)
					return item.value.propListModel;
				else
					return null;
			});
			selectedItemProtoParamListModel = Qt.binding(function() {
				var item = getCurrentItem();
				if (item && item.type === ObjectProperty.Object)
					return item.value.protoPropListModel;
				else
					return null;
			});
			selectedItemArrayListModel = Qt.binding(function() {
				var item = getCurrentItem();
				if (item && item.type === ObjectProperty.List)
					return item.value.listModel;
				else
					return null;
			});
			getSelectedItemPropListBlockState = function() {
				var item = getCurrentItem();
				if (item.type === ObjectProperty.List)
					return "list";
				else
					return "object";
			}
			getSelectedItemState = function() {
				var item = getCurrentItem();
				if (item.type === ObjectProperty.List)
					return "list";
				else if (item.type === ObjectProperty.Object)
					return "object";
				else
					return "simpleType";
			}
			selectedItemSimpleValue = Qt.binding(function() {
				var item = getCurrentItem();
				if (item)
					return item.valueStr
				return "<null>";
			});
			varListFooterAddClicked = function(listModel, type) {
				listModel.add(type);
			}
		}

		property var onModelLoaded: function(fPath) {
			console.log("DMBModel: File loaded '" + fPath + "'");
			console.log("fname: " + app.nameFromUrl(fPath));
			bodyBlock.stateOpened();
			// ======= Properties for Content functional =======
			contentListModel = dmbModel.contentModel.propListModel;
			contentDeleteItemClicked = contentListModel.removeAt;
			// ======= End of properties for Content functional =======
			setSelectedObjectBindings();
			setSelectedItemBindings();
			var relPath = app.relPathFromUrl(fPath);
			if (!dmbModelRecentFiles.contentModel.get("info").has(relPath))
			{
				var o = dmbModelRecentFiles.createObject();
				o.set("name", app.nameFromUrl(fPath));
				o.set("relPath", relPath);
				o.set("url", fPath);
				dmbModelRecentFiles.contentModel.get("info").set(relPath, o);
				recentFilesModel.add(relPath, 0);
			}
			else
			{
				var list = dmbModelRecentFiles.contentModel.get("list");
				var entry = list.find(fPath);
				if (entry)
					entry.remove();
				list.add(fPath, 0);
				console.log("Already exists");
			}
			dmbModelRecentFiles.store();
		}

		property var onModelLoadError: function(error) {
			console.log("Load file error: " + error);
        }

		property var onRecentFilesModelLoaded: function(fPath) {
			console.log("onRecentFilesModelLoaded('" + fPath + "')");
			recentFilesModel = dmbModelRecentFiles.contentModel.get("list").listModel
		}

		property var onRecentFilesModelLoadError: function(error) {
			console.log("onRecentFilesModelLoadError('" + error + "')");
		}

		Component.onCompleted: {
			if (dmbModel.isLoaded)
				bodyBlock.stateLoaded();
			else
			{
				var fPath = "recent.json";
				if (!dmbModelRecentFiles.load(fPath))
				{
					console.log("Create recent files storage recent.json");
					dmbModelRecentFiles.contentModel.add("list", ObjectProperty.List);
					dmbModelRecentFiles.contentModel.add("info", ObjectProperty.Object);
					dmbModelRecentFiles.store(fPath);
				}
				bodyBlock.stateChooseFile();
			}
//			if (!dmbModel.load("model.json"))
//				dmbModel.store("model.json", true);
			// onModelLoaded should be called in success
		}

    }
}
