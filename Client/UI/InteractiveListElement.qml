import QtQuick
import QtQuick.Controls

VarInterface {
	id: item1
	width: parent.width
	height: 40
	state: "base"

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

	property var menuModel: null

	property var onPress: function () {
		//console.log("InteraciveListElement.onPress default handler");
	}

	property var onClick: function () {
		//console.log("InteraciveListElement.onClick default handler");
	}


	MouseArea {
		id: mouseArea
		anchors.fill: parent
		acceptedButtons: Qt.LeftButton | Qt.RightButton
		propagateComposedEvents: true
		onPressed: function(mouse){
			if (container)
				container.currentIndex = index
			if (mouse.button === Qt.RightButton)
			{
				if (menuModel)
					contextMenu.popup();
			}
			else if (mouse.button === Qt.LeftButton)
				onPress();
		}
		onClicked: function(mouse) {
			if (mouse.button === Qt.LeftButton)
				onClick();
		}

		onPressAndHold: function(mouse){
			if (mouse.source === Qt.MouseEventNotSynthesized)
				if (menuModel)
					contextMenu.popup();
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




