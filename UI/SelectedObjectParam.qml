import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    x: 5
    width: 150
    height: 40

    property var rowIndex: index
    Row {
        id: row1
        spacing: 10


        Text {
            id: paramName
            text: name
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }

        ComboBox {
            id: paramType
            width: 103
            height: 26
            model: selectedObjParamTypeModel
            onActivated: selectedObjectParameterTypeChanged(rowIndex, currentIndex, currentValue)
            Component.onCompleted: currentIndex = selectedObjectParamType(rowIndex)
        }
    }

    MouseArea {
        width: 50
        height: 50
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: function(mouse){
            params.currentIndex = index
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
                onClicked: selectedObjectDeleteParameterClicked(index)
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}D{i:2}D{i:3}D{i:1}D{i:5}D{i:4}
}
##^##*/
