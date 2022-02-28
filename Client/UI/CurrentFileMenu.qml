import QtQuick
import QtQuick.Controls

FileMenu {
	id: fileMenu
	MenuSeparator {}

	MenuItem {
		text: qsTr("Close")
		onClicked: closeFileClicked()
	}

	MenuItem {
		text: qsTr("Save")
		onClicked: saveClicked()
	}
	MenuItem {
		text: qsTr("Save as")
		onClicked: saveAsClicked()
	}
}
