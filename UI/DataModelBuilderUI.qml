import QtQuick
import DataModelBuilderUI 1.0

Item {
    width: Constants.width
    height: Constants.height
    property alias objectsLibrary: objectsLibrary
    Screen01 {
        ObjectsLibrary {
            id: objectsLibrary
            x: 39
            y: 8
            height: parent.height
        }

        SelectedObject {
            id: selectedObject
            x: 432
            y: 8
            height: parent.height
        }
    }
}


