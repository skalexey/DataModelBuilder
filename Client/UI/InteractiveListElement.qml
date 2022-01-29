import QtQuick
import QtQuick.Controls

Item {
	id: item1
	width: parent.width
	height: 40
	state: "base"

	property int rowIndex: index
	property bool ignoreChanges: false
	property var container: null
	property alias mouseArea: mouseArea
	property alias contextMenu: contextMenu

	property var menuDelegate: Component {
		MenuItem {
			text: title
			onTriggered: {
				cmd();
			}
		}
	}

	property var menuModel: ListModel {
		ListElement {
			title: "Test menu element 1"
		}
		ListElement {
			title: "Test menu element 2"
		}
	}

	MouseArea {
		id: mouseArea
		anchors.fill: parent
		acceptedButtons: Qt.LeftButton | Qt.RightButton
		propagateComposedEvents: true
		onPressed: function(mouse){
			container.currentIndex = index
			if (mouse.button === Qt.RightButton)
				contextMenu.popup()
		}
		onPressAndHold: function(mouse){
			if (mouse.source === Qt.MouseEventNotSynthesized)
				contextMenu.popup()
		}

		Menu {
			id: contextMenu
			Instantiator {
				model: menuModel
				onObjectAdded: function(i, o) {
					contextMenu.insertItem(i, o);
				}
				onObjectRemoved: function(o) {
					contextMenu.removeItem(o);
				}
				delegate: menuDelegate
			}
		}
	}
}




