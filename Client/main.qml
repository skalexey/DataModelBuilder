import QtQuick
import ObjectModel 1.0
import DataModelBuilderUI 1.0

Item {
    width: Constants.width
    height: Constants.height

    // To be used in libraryObjectListModel.onRowsRemoved
    property var rowsRemovedF: dataModelBuilderUI.onObjectRemoved

    DataModelBuilderUI {
        id: dataModelBuilderUI
        function getCurrentObj() {
            return libraryObjectListModel.object(libraryCurrentObjectIndex);
        }

        // When an object is removed, selectedObject should get the new data
        function setSelectedObjectBindings() {
            selectedObjectName = Qt.binding(function() { return getCurrentObj().name })
            selectedObjParamTypeModel = paramTypeModel
            selectedObjectParamType = function (paramIndex) { return getCurrentObj().param(paramIndex).type }
            selectedObjectNewParamClicked = function() { getCurrentObj().newParam() }
            selectedObjectDeleteParameterClicked = function(i) { getCurrentObj().removeParam(i) }
            selectedObjectParameterTypeChanged = function(paramIndex, i, val) { getCurrentObj().param(paramIndex).type = i }
            selectedObjParamListModel = Qt.binding(function() { return getCurrentObj().paramListModel })
        }

        property var onObjectRemoved: function() {
            setSelectedObjectBindings()
        }

        Component.onCompleted: {
            setSelectedObjectBindings()
        }

        libraryObjectListModel: ObjectListModel {
            id: m
            onRowsRemoved: function () { rowsRemovedF() }
        }
        libraryAddNewObjectClicked: m.newObject
        libraryDeleteObjectClicked: function(i) { m.removeObject(i) }
    }
}
