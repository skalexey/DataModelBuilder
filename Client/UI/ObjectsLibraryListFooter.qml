import QtQuick
import QtQuick.Controls

Column {
	Button {
		id: btnAddNewObject
		text: qsTr("Add")
		onClicked: contextMenu.popup();
	}

	Menu {
		id: contextMenu

		MenuItem {
			text: qsTr("New type")
			onTriggered: libraryAddNewObjectClicked()
		}

		TypeInstanceMenu {
			onClicked: function(propId) {
				libraryTypeListModel.parent.instantiate(propId);
			};
		}
	}
}
