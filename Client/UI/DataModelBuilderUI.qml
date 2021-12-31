import QtQuick
import DataModelBuilderUI 1.0

Item {
    width: Constants.width
    height: Constants.height
    property int libraryCurrentObjectIndex: objectsLibrary.libraryContainer.currentIndex
    property var libraryAddNewObjectClicked: function() { console.log("Default click handler") }
    property var libraryDeleteObjectClicked: function(i) { console.log("DataModelBuilderUI: onDeleteObjectClicked(" + i + ") default handler") }
    property var libraryObjectListModel: ListModel {
        ListElement {
            name: "Scheme element"
        }

        ListElement {
            name: "Connection"
        }

        ListElement {
            name: "Back image"
        }

        ListElement {
            name: "Square"
        }
    }
    property var selectedObjParamTypeModel: ["No modifier", "Shift", "Control"]
    property string selectedObjectName: "Obj name"
    property var selectedObjectParamType: function(i) { return 2 }
    property var selectedObjectNewParamClicked: function() { console.log("DataModelBuilderUI: onNewParamClicked default handler") }
    property var selectedObjectDeleteParameterClicked: function(i) { console.log("DataModelBuilderUI: onDeleteParameterClicked(" + i + ") Delete param default handler") }
    property var selectedObjectParameterTypeChanged: function(paramIndex, i, val) { console.log("DataModelBuilderUI: onParameterTypeChanged(" + paramIndex + ", " + i + ", '" + val + "') default handler") }
    property var selectedObjParamListModel: ListModel {
        ListElement {
            name: "Type 1"
        }

        ListElement {
            name: "Type 2"
        }

        ListElement {
            name: "Type 3"
        }
    }

    Screen01 {
        ObjectsLibrary {
            id: objectsLibrary
            x: 39
            y: 8
            height: parent.height
        }

        SelectedObject {
            x: 432
            y: 8
            height: parent.height
        }
    }
}


