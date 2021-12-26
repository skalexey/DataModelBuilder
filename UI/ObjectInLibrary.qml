import QtQuick
import QtQuick.Controls

Item {
    id: obj
    x: 5
    height: 50
    property var parentContainer: objectsLibraryContainer
    Column {
        spacing: 5
        Text {
            x: 5
            text: name
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
    MouseArea {
        width: 50
        height: 50
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: function(mouse){
            parentContainer.currentIndex = index
            if (mouse.button === Qt.RightButton)
                contextMenu.popup()
        }
        onPressAndHold: function(mouse){
            if (mouse.source === Qt.MouseEventNotSynthesized)
                contextMenu.popup()
        }
        Menu {
            id: contextMenu
            MenuItem {
                text: "Rename"
            }
            MenuItem {
                text: "Delete"
                onClicked: libraryDeleteObjectClicked(index)
            }
        }
    }
}
