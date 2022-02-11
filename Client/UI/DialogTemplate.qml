import QtQuick
import QtQuick.Controls

Dialog {
	property var onShow: function() {}
	property var show: function() {
		visible = true;
		onShow();
	}
	standardButtons: Dialog.Ok | Dialog.Cancel
	modal: true
	x: root.width / 2 - width / 2
	y: root.height / 2 - height / 2
	width: 300
	height: 220
}
