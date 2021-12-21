import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: obj
    x: 5
    height: 50
    property alias obj: obj
    property alias objectName: objectName
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
}
