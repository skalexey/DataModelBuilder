import QtQuick
import QtQuick.Controls
import DataModelBuilderUI 1.0

Rectangle {
    width: Constants.width
    height: Constants.height

    color: Constants.backgroundColor

    Text {
        text: qsTr("Hello DataModelBuilderUI")
        anchors.centerIn: parent
        font.family: Constants.font.family
    }
}
