import QtQuick
import QtQuick.Controls

Column {
	property var listModel: null
	Component.onCompleted: {
		console.log("addBtn.height: " + addBtn.height)
	}

	Button {
		id: addBtn
		text: qsTr("Add")
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
}
/*##^##
Designer {
	D{i:0;autoSize:true;height:480;width:640}D{i:1}
}
##^##*/
