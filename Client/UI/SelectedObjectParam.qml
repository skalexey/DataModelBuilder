import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: item1
    x: 5
    width: 150
    height: 40
    state: "base"

    property var rowIndex: index
    property bool ignoreChanges: false

    Row {
        id: row1
        spacing: 10


        Text {
            id: paramName
            visible: true
            text: name
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }

        TextField {
            id: textField
            width: 100
            height: 25
            text: name
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
            model: selectedObjParamTypeModel
            onActivated: type = currentValue
            Component.onCompleted: currentIndex = type
        }

    }

    MouseArea {
        width: 50
        height: 50
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



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2}D{i:3}D{i:4}D{i:1}D{i:6}D{i:5}
}
##^##*/
