import QtQuick
import QtQuick.Controls

Column {
    Button {
        id: btnAddNewObject
        text: qsTr("Add new")
        onClicked: libraryAddNewObjectClicked()
    }

	Item {
		id: spacer
		height: 12
		width: parent.width
	}

	TypeInstanceControl {
		buttonClick: function(propId) {
			libraryTypeListModel.parent.instantiate(propId);
		};
	}
}
