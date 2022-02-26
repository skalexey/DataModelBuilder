import QtQuick
import QVL 1.0
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

		onModelLoaded: function(f) {
			uiRoot.onModelLoaded(f);
		}
		onModelLoadError: function(f, e) {
			uiRoot.onModelLoadError(f, e);
		}

		onModelStored: function(f) {
			uiRoot.modelStored(f);
		}

		onModelStoreError: function(f, e) {
			uiRoot.modelStoreError(f, e);
		}
	}

	DMBModel {
		id: dmbModelRecentFiles
		onModelLoaded: function (f) {
			uiRoot.onRecentFilesModelLoaded(f);
		}
		onModelLoadError: function(f, e) {
			uiRoot.onRecentFilesModelLoadError(f, e);
		}
	}

    DataModelBuilderUI {
        id: uiRoot
        width: root.width
        height: root.height
		typesListsModelDetailed: ObjectProperty.vlTypeModelDetailed
		typesListsModel: ObjectProperty.vlTypeModel
		// ======= Menu properties ========
		property var currentlyOpenedFile: "";
		openFileClicked: function() {
			chooseFileDialog.show();
		}

		saveAsClicked: function() {
			uiRoot.saveAsDialog.show();
			uiRoot.saveAsDialog.initialText = "";
			uiRoot.saveAsDialog.onConfirm = function(enteredText) {
				console.log("Save current model to a file '" + enteredText + "'");
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
		selectedObjParamListModel: null
		textCurrentFile: ""

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
				return fPath ? fPath : typeof fPath;
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
			selectedObjectName = Qt.binding(function() {
				var obj = getCurrentObj();
				return obj ? obj.name : "";
			});
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
				{
					var model = obj.value.propListModel;
					connectInstantiation(model.parent);
					return model;
				}
				else
					return new null;
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
				{
					connectInstantiation(item.value);
					return item.value.propListModel;
				}
				else
					return null;
			});
			selectedItemProtoParamListModel = Qt.binding(function() {
				var item = getCurrentItem();
				if (item && item.type === ObjectProperty.Object)
				{
					connectInstantiation(item.value);
					return item.value.protoPropListModel;
				}
				else
					return null;
			});
			selectedItemArrayListModel = Qt.binding(function() {
				var item = getCurrentItem();
				if (item && item.type === ObjectProperty.List)
				{
					var model = item.value.listModel;
					return model;
				}
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
			selectedItemSimpleValue = function() {
				var item = getCurrentItem();
				if (item)
					return item.valueStr
				return "<null>";
			};
			selectedItemValue = function() {
				var item = getCurrentItem();
				if (item)
					return item.value;
				return "<null>";
			};
			selectedItemChangeValue = function(val) {
				var item = getCurrentItem();
				if (item)
					item.value = val;
			};
			selectedItemChangeName = function(val) {
				var item = getCurrentItem();
				if (item)
					item.name = val;
			};
			getSelectedItemType = function() {
				var item = getCurrentItem();
				if (item)
					return item.type;
				else
					return "";
			};
			getSelectedItemTypeStr = function() {
				var item = getCurrentItem();
				if (item)
					return item.typeStr;
				else
					return "";
			}
			getSelectedItemParentModel = function() {
				var item = getCurrentItem();
				if (item)
					return item.parentModel;
				else
					return null;
			}

			setSelectedItemType = function(val) {
				var item = getCurrentItem();
				if (item)
					item.type = val;
			};

			varListFooterAddClicked = function(listModel, type) {
				listModel.add(type);
			}
		}

		function storeRecentFile(fPath) {
			console.log("Store recent file '" + fPath + "'");
			var relPath = app.relPath(fPath);
			textCurrentFile = relPath;
			if (!dmbModelRecentFiles.contentModel.get("info").has(relPath))
			{
				var o = dmbModelRecentFiles.createObject();
				o.set("name", app.nameFromUrl(fPath));
				o.set("relPath", relPath);
				o.set("url", fPath);
				dmbModelRecentFiles.contentModel.get("info").set(relPath, o);
				uiRoot.recentFilesModel.add(relPath, 0);
			}
			else
			{
				console.log("Already exists. Put on top");
				var list = dmbModelRecentFiles.contentModel.get("list");
				var entry = list.find(relPath);
				if (entry)
					entry.remove();
				list.add(relPath, 0);
			}
			dmbModelRecentFiles.store();
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
			contentListModel.dataChanged.connect(function(indexTopLeft, indexBottomRight, roles) {
				setSelectedItemBindings();
			});

			connectInstantiation(dmbModel.contentModel);
			connectInstantiation(dmbModel.typesModel);
			storeRecentFile(fPath);
		}

		property var onModelLoadError: function(f, error) {
			console.log("Load file error: " + error);
        }

		property var onRecentFilesModelLoaded: function(fPath) {
			console.log("onRecentFilesModelLoaded('" + fPath + "')");
			recentFilesModel = dmbModelRecentFiles.contentModel.get("list").listModel
		}

		QtObject {
			id: local
			property bool recentFilesLoadError: false
		}

		property var onRecentFilesModelLoadError: function(f, error) {
			if (!local.recentFilesLoadError)
			{
				local.recentFilesLoadError = true;
				console.log("Create recent files storage recent.json");
				dmbModelRecentFiles.contentModel.add("list", ObjectProperty.List);
				dmbModelRecentFiles.contentModel.add("info", ObjectProperty.Object);
				if (!dmbModelRecentFiles.store(f))
					onRecentFilesModelLoadError(f, "Can't store recent files storage");
				else
					onRecentFilesModelLoaded(f);
			}
			else
				console.log("Load recent files storage error: " + error);
		}

		property var modelStored: function(f) {
			storeRecentFile(f);
		}

		property var modelStoreError: function(f, e) {
			console.log("Store model error: " + e);
		}

		Component.onCompleted: {
			if (dmbModel.isLoaded)
				bodyBlock.stateLoaded();
			else
			{
				var fPath = "recent.json";
				dmbModelRecentFiles.load(fPath);

				bodyBlock.stateChooseFile();
			}
//			if (!dmbModel.load("model.json"))
//				dmbModel.store("model.json", true);
			// onModelLoaded should be called in success
		}

    }
}
