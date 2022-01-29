import QtQuick
import QtQuick.Controls

Item {
    id: obj
    x: 0
    height: 50
    width: parent.width
    state: "base"
    property var parentContainer: objectsLibraryContainer
    property bool ignoreChanges: false

    Text {
        leftPadding: 6
        id: nameText
        text: name
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
    }

    TextField {
        id: textField
        width: 100
        height: 25
        visible: false
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
                typeClick();
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
                text: "Instantiate"
                onClicked: instantiateClick(parentContainer.currentIndex)
            }
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
    D{i:0;formeditorZoom:0.75}D{i:1}D{i:2}D{i:4}D{i:3}
}
##^##*/
