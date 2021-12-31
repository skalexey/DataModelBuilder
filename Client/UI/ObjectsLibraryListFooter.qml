import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    Button {
        id: btnAddNewObject
        text: qsTr("Add new")
        onClicked: libraryAddNewObjectClicked()
    }
}
