import QtQuick
import QtQuick.Controls

Item {
	id: control
	width: instantiateType.width
	height: instantiateType.height

	property var buttonClick: function(propId) {
		console.log("TypeInstanceControl.buttonClick(" + propId + ") default handler");
	}

	Button {
		id: instantiateType
		text: "Add a type instance"
		onClicked: {
			contextMenu.popup();
		}
	}
	Menu {
		id: contextMenu
		Instantiator {
			model: libraryTypeListModel
			onObjectAdded: function(i, o) {
				contextMenu.insertItem(i, o);
			}
			onObjectRemoved: function(o) {
				contextMenu.removeItem(o);
			}
			delegate: Component {
				MenuItem {
					text: name
					onTriggered: {
						control.buttonClick(name);
					}
				}
			}
		}
	}
}
