import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 500
    height: 1080

    Text {
        id: objectName
        x: 163
        y: 75
        text: qsTr("Object Name")
        font.pixelSize: 30
    }

    ListView {
        id: params
        x: 195
        y: 149
        width: 110
        height: 160
        delegate: SelectedObjectParam {}
        model: ListModel {
            ListElement {
                name: "Name"
            }

            ListElement {
                name: "ImageUrl"
            }

            ListElement {
                name: "Circled"
            }
        }
    }
}
