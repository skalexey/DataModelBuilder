import QtQuick
import QtQuick.Controls

Item {
	property var listModel: null
	Button {
		id: addToList
		text: qsTr("Add")
		onClicked: {
			contextMenu.popup();
		}
	}
	Menu {
		id: contextMenu
		Instantiator {
			model: vlTypeModelDetailed
			onObjectAdded: function(i, o) {
				contextMenu.insertItem(i, o);
			}
			onObjectRemoved: function(o) {
				contextMenu.removeItem(o);
			}
			delegate: Component {
				MenuItem {
					text: Object.keys(modelData)[0]
					onTriggered: {
						cmd(varListFooterAddClicked(listModel, modelData[Object.keys(modelData)[0]]));
					}
				}
			}
		}
	}
}
/*##^##
Designer {
	D{i:0;autoSize:true;height:480;width:640}D{i:1}
}
##^##*/
