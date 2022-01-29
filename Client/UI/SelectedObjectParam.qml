import QtQuick
import QtQuick.Controls

Item {
    id: item1
    width: parent.width
    height: 40
    state: "base"

    property var rowIndex: index
    property bool ignoreChanges: false

    Text {
        id: paramName
        visible: true
        text: name
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 6
        font.bold: true
    }

    TextField {
        id: textField
        width: paramName.width
        height: 25
        text: name
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 54
        placeholderText: qsTr("Text Field")
        onFocusChanged: {
            if (!focus)
            {
                if (!ignoreChanges)
                    name = text;
                item1.state = "base"
            }
        }
        Keys.onEscapePressed: {
            ignoreChanges = true;
            item1.state = "base"
            ignoreChanges = false;
        }
        function enterPressed() {
            textField.focus = false;
            item1.state = "base";
        }

        Keys.onEnterPressed: enterPressed()
        Keys.onReturnPressed: enterPressed()
    }

    ComboBox {
        id: paramType
        width: 103
        height: 26
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 6
        model: selectedObjParamTypeModel
        onActivated: type = currentValue
        Component.onCompleted: currentIndex = type
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        propagateComposedEvents: true
        onPressed: function(mouse){
            params.currentIndex = index
            if (mouse.button === Qt.RightButton)
                contextMenu.popup()
        }
        onPressAndHold: function(mouse){
            if (mouse.source === Qt.MouseEventNotSynthesized)
                contextMenu.popup()
        }
        onDoubleClicked: function(mouse) {
            textField.focus = true
            item1.state = "edit"
            textField.focus = true
        }

        Menu {
            id: contextMenu
            MenuItem {
                text: "Rename"
                onClicked: {
                    item1.state = "edit"
                    textField.focus = true
                }
            }
            MenuItem {
                text: "Delete"
                onClicked: selectedObjectDeleteParameterClicked(index)
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
                target: paramName
                visible: false
            }
        }
    ]
}




