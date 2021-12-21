import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    x: 5
    width: 150
    height: 40
    property var typeModel
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
            model: typeModel
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}D{i:2}D{i:3}D{i:1}
}
##^##*/
