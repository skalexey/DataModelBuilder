import QtQuick
import QtQuick.Controls

Menu {
	title: "Add a type instance"
	id: contextMenu

	function log(msg) {
		console.log("TypeInstanceControl: " + msg);
	}

	property var onClicked: function(name) {
		log("onClicked(" + name + ") default handler");
	}

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
					contextMenu.onClicked(name);
				}
			}
		}
	}
}
