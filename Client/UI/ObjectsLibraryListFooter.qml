import QtQuick
import QtQuick.Controls

Item {
    Button {
        id: btnAddNewObject
        text: qsTr("Add new")
        onClicked: libraryAddNewObjectClicked()
    }
}
