import QtQuick 2.4
import QtQuick.Controls 2.15

Item {
    width: 640
    height: 480
    visible: true
    property alias gview: gridView
    property alias selectedItem: selectedItem
    property alias objectListModel: gridView.model
    GridView {
        id: gridView
        x: 31
        y: 79
        width: 151
        height: 374
        delegate: Item {
            x: 5
            height: 50
            Column {
                Image {
                    id: img
                    source: ""
                    width: 40
                    height: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    x: 5
                    text: name
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                spacing: 5
            }
        }
        cellHeight: 70
        cellWidth: 70
        model: ListModel {
            ListElement {
                name: "Element 1"
            }
            ListElement {
                name: "Element 2"
            }
            ListElement {
                name: "Element 3"
            }
        }
    }

    TextInput {
        id: textInput
        x: 45
        y: 41
        width: 80
        height: 20
        text: qsTr("Items Library")
        font.pixelSize: 12
    }

    Item {
        id: selectedItem
        x: 196
        y: 70
        width: 248
        height: 374

        Text {
            id: itemName
            x: 37
            y: 32
            width: 125
            height: 24
            text: qsTr("Item name")
            font.pixelSize: 12
        }

        ListView {
            id: properties
            x: 45
            y: 107
            width: 110
            height: 160
            delegate: Item {
                id: item1
                x: 5
                width: 80
                height: 40
                Row {
                    id: row1
                    spacing: 10

                    Text {
                        id: paramName
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: true
                    }

                    Text {
                        id: text1
                        text: qsTr("Text")
                        font.pixelSize: 12
                    }
                }
            }
            model: ListModel {
                ListElement {
                    name: "Grey"
                    colorCode: "grey"
                }

                ListElement {
                    name: "Red"
                    colorCode: "red"
                }

                ListElement {
                    name: "Blue"
                    colorCode: "blue"
                }

                ListElement {
                    name: "Green"
                    colorCode: "green"
                }
            }
        }
    }

    Rectangle {
        id: rectangle
        x: 179
        y: 36
        width: 61
        height: 25
        color: "#ffffff"
    }

    Button {
        id: addObject
        x: 208
        y: 37
        text: qsTr("Button")

        Connections {
            target: addObject
            onClicked: objectListModel.addItem()
        }
    }
}
