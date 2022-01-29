import QtQuick
import QtQuick.Controls

Item {
    id: obj
    width: parent.width
    height: 40
    state: "base"

    property var parentContainer: contentBlockContainer
    property bool ignoreChanges: false

    Text {
        id: text1
        text: name
        anchors.verticalCenter: parent.verticalCenter
        leftPadding: 6
        anchors.left: parent.left
        font.bold: true
    }

    TextField {
        id: textField
        width: 100
        height: 25
        text: name
        anchors.verticalCenter: parent.verticalCenter
        placeholderText: qsTr("Text Field")
        Keys.onReturnPressed: focus = false;
        Keys.onEnterPressed: focus = false;
        Keys.onEscapePressed: function() {
            ignoreChanges = true;
            focus = false;
            ignoreChanges = false;
        }
        onFocusChanged: function() {
            if (!focus)
            {
                if (!ignoreChanges)
                    name = text
                obj.state = "base"
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        propagateComposedEvents: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: function(mouse) {
            parentContainer.currentIndex = index
            if (mouse.button === Qt.RightButton)
                contextMenu.popup();
        }
        onClicked: function(mouse) {
            if (mouse.button === Qt.LeftButton)
                itemClick();
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
                    textField.focus = true;
                }
            }
            MenuItem {
                text: "Delete"
                onClicked: contentDeleteItemClicked(index)
            }
        }
    }

    states: [
        State {
            name: "base"

            PropertyChanges {
                target: textField
                visible: false
            }
        },
        State {
            name: "edit"

            PropertyChanges {
                target: text1
                visible: false
            }

            PropertyChanges {
                target: textField
                focus: true
            }
        }
    ]
}
