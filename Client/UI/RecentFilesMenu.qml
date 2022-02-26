import QtQuick
import QtQuick.Controls

Menu {
	id: recentFilesMenu
	title: "Recent files"

	property var model: null

	Instantiator {
		model: recentFilesMenu.model
		onObjectAdded: function(i, o) {
//			var zeroItem = recentFilesMenu.items[0];
//			if (zeroItem && zeroItem.isNoItems)
//				recentFilesMenu.removeItem(zeroItem);
			recentFilesMenu.insertItem(i, o);
		}
		onObjectRemoved: function(o) {
			recentFilesMenu.removeItem(o);
		}
		delegate: MenuItem {
			property bool isNoItems: false
			text: valueStr
			onTriggered: function() {
				onOpenFile(value);
			}
		}
	}
//	Component.onCompleted: function() {
//		recentFilesMenu.insertItem(0, Qt.createQmlObject("import QtQuick;import QtQuick.Controls; MenuItem {property bool isNoItems: true; text: qsTr('No recent files')}", recentFilesMenu));
//	}
}
