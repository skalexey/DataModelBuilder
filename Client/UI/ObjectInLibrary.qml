import QtQuick
import QtQuick.Controls

Item {
    id: obj
    x: 5
    height: 50
    state: "base"
    property var parentContainer: objectsLibraryContainer
    property var exitHolder
    Column {
        spacing: 5
        Text {
            id: nameText
            x: 5
            text: name
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextField {
            id: textField
            y: 0
            width: 100
            height: 25
            visible: false
            text: name
            placeholderText: qsTr("Text Field")
            Keys.onReturnPressed: focus = false;
            Keys.onEnterPressed: focus = false;
            Keys.onEscapePressed: function() {
                exitHolder = true;
                focus = false;
                exitHolder = false;
            }
            onFocusChanged: function() {
                if (!focus)
                {
                    if (!exitHolder)
                        name = text
                    obj.state = "base"
                }
            }
        }
    }
    MouseArea {
        width: 50
        height: 50
        propagateComposedEvents: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: function(mouse) {
            parentContainer.currentIndex = index
            if (mouse.button === Qt.RightButton)
                contextMenu.popup();
        }
        onDoubleClicked: function(mouse) {
            obj.state = "edit"
            textField.focus = true;
        }
        onPressAndHold: function(mouse) {
            if (mouse.source === Qt.MouseEventNotSynthesized)
                contextMenu.popup()
        }
        Menu {
            id: contextMenu
            MenuItem {
                text: "Rename"
                onClicked: function () {
                    obj.state = "edit"
                }
            }
            MenuItem {
                text: "Delete"
                onClicked: libraryDeleteObjectClicked(index)
            }
        }
    }
    states: [
        State {
            name: "edit"

            PropertyChanges {
                target: textField
                visible: true
            }

            PropertyChanges {
                target: nameText
                visible: false
            }
        },
        State {
            name: "base"
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}D{i:2}D{i:3}D{i:1}D{i:5}D{i:4}
}
##^##*/
