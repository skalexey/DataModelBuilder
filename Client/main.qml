import QtQuick
import TypeModel 1.0
import DataModelBuilderUI 1.0

Item {
    width: Constants.width
    height: Constants.height

    // To be used in libraryTypeListModel.onRowsRemoved
    property var rowsRemovedF: dataModelBuilderUI.onObjectRemoved

    DataModelBuilderUI {
        id: dataModelBuilderUI
        function getCurrentObj() {
            console.log("libraryTypeListModel.object(" + libraryCurrentObjectIndex + ")");
            var obj = libraryTypeListModel.object(libraryCurrentObjectIndex);
            if (!obj)
                console.log("NULL in getCurrentObj()!");
            return obj;
        }

        // When an object is removed, selectedObject should get the new data
        function setSelectedObjectBindings() {
            selectedObjectName = Qt.binding(function() {
                var obj = getCurrentObj();
                return obj ? obj.name : "";
            });
            selectedObjParamTypeModel = propTypeModel;
            selectedObjectNewParamClicked = function() {
                var obj = getCurrentObj();
                if (obj)
                    obj.newParam();
            }
            selectedObjectDeleteParameterClicked = function(i) {
                var obj = getCurrentObj();
                if (obj)
                    obj.removeParam(i);
            }
            selectedObjParamListModel = Qt.binding(function() {
                var obj = getCurrentObj();
                if (obj)
                    return obj.paramListModel;
                else
                    return new ObjectModel;
            })
        }

        property var onObjectRemoved: function() {
            setSelectedObjectBindings()
        }

        Component.onCompleted: {
            setSelectedObjectBindings()
        }

        libraryTypeListModel: TypeListModel {
            id: m
            onRowsRemoved: function () { rowsRemovedF() }
        }
        libraryAddNewObjectClicked: m.newObject
        libraryDeleteObjectClicked: function(i) { m.removeObject(i) }
        storeClicked: m.store
    }
}
