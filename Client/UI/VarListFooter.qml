import QtQuick
import QtQuick.Controls

Column {
	property var listModel: null
	property string buttonText: qsTr("Add")

	Button {
		id: addBtn
		text: buttonText
		onClicked: {
			contextMenu.popup();
		}
	}
	Menu {
		id: contextMenu
		Instantiator {
			model: typesListsModelDetailed
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
						varListFooterAddClicked(listModel, modelData[Object.keys(modelData)[0]]);
					}
				}
			}
		}
	}
	TypeInstanceControl {
		buttonClick: function(propId) {
			listModel.parent.instantiate(propId);
		};
	}
}
/*##^##
Designer {
	D{i:0;autoSize:true;height:480;width:640}D{i:1}
}
##^##*/
