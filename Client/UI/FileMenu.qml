import QtQuick
import QtQuick.Controls

Menu {
	id: fileMenu
	MenuItem {
		text: qsTr("New")
		onClicked: newClicked()
	}

	MenuItem {
		text: qsTr("Open")
		onClicked: openFileClicked()
	}
}
