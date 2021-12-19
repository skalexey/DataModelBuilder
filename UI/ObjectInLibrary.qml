import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: item1
    x: 5
    height: 50
    Column {
        spacing: 5

        Text {
            id: objectName
            x: 5
            text: name
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
    states: [
        State {
            name: "selected"
        }
    ]
}
