import QtQuick
import DataModelBuilderUI 1.0

Item {
    width: Constants.width
    height: Constants.height

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

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}D{i:2}D{i:3}D{i:1}
}
##^##*/
