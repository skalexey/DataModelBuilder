import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 300
    height: 1080

    property alias gridView: objectsLibraryContainer

    Text {
        id: objectsLibraryTitle
        x: 51
        y: 49
        text: qsTr("Objects Library")
        font.pixelSize: 30
    }

    ListView {
        id: objectsLibraryContainer
        x: 33
        y: 136
        width: 237
        height: 900
        delegate: ObjectInLibrary {
            id: obj
            MouseArea {
                width: 50
                height: 50
                onClicked: objectsLibraryContainer.currentIndex = index
            }
        }
        highlight: Rectangle {
            color: "cyan"
            width: parent.width
            height: objectsLibraryContainer.currentItem.height
            y: objectsLibraryContainer.currentItem.y;
        }
        highlightFollowsCurrentItem: false
        focus: true
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
    }
}
