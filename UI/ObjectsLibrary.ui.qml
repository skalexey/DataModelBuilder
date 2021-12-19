import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 300
    height: 1080

    Text {
        id: objectsLibraryTitle
        x: 51
        y: 49
        text: qsTr("Objects Library")
        font.pixelSize: 30
    }

    GridView {
        id: objectsLibraryContainer
        x: 33
        y: 136
        width: 237
        height: 900
        delegate: ObjectInLibrary {}
        cellHeight: 100
        model: ListModel {
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
        cellWidth: 100
    }
}
