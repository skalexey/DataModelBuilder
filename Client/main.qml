import QtQuick
import DMBModel 1.0
import DataModelBuilderUI 1.0

Window {
    id: root
    width: Constants.width
    height: Constants.height
    visible: true
    // To be used in libraryTypeListModel.onRowsRemoved
    property var rowsRemovedF: uiRoot.onObjectRemoved
	property var modelLoadedF: uiRoot.onModelLoaded

	DMBModel {
		id: dmbModel
		onInstantiateRequested: function(instId, protoId) {
			uiRoot.preInstantiateDialog.visible = true
			uiRoot.preInstantiateDialog.instanceName = instId
			uiRoot.preInstantiateDialog.onConfirm = function(enteredText) {
				console.log("Instantiate '" + enteredText + "' of type '" + protoId + "'")
				dmbModel.addToContent(enteredText, protoId)
			}

		}
		onInstantiateRefused: function(error) {
			console.log("Instantiate refused: " + error);
		}
		onModelLoaded: function(f) {
			uiRoot.onModelLoaded(f)
		}
	}

    DataModelBuilderUI {
        id: uiRoot
        width: root.width
        height: root.height
		// ======= Properties for Type library functional =======
		// Reset default models
		libraryTypeListModel: null
		contentListModel: null
		selectedItemOwnParamListModel: null
		selectedItemProtoParamListModel: null
		selectedItemArrayListModel: null

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
			selectedObjParamTypeModel = vlTypeModel;
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
			// ======= Properties for Content functional =======
			contentListModel = dmbModel.contentModel.propListModel;
			contentDeleteItemClicked = contentListModel.removeAt;
			// ======= End of properties for Content functional =======

			setSelectedObjectBindings();
			setSelectedItemBindings();
		}

        // ======= End of Properties for Selected content item =======
        // ======= Common code =======
        Component.onCompleted: {
			if (!dmbModel.load("model.json"))
				dmbModel.store("model.json", true);
			// onModelLoaded should be called in success
        }
    }
}
