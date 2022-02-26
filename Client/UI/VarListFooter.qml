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
		Menu {
			title: "Basic type"
			id: basicTypesMenu
			Instantiator {
				model: typesListsModelDetailed
				onObjectAdded: function(i, o) {
					basicTypesMenu.insertItem(i, o);
				}
				onObjectRemoved: function(o) {
					basicTypesMenu.removeItem(o);
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
		TypeInstanceMenu {
			onClicked: function(propId) {
				listModel.parent.instantiate(propId);
			};
		}
	}

}
/*##^##
Designer {
	D{i:0;autoSize:true;height:480;width:640}D{i:1}
}
##^##*/
