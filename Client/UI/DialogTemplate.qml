import QtQuick
import QtQuick.Controls

Dialog {
	standardButtons: Dialog.Ok | Dialog.Cancel
	modal: true
	x: root.width / 2 - width / 2
	y: root.height / 2 - height / 2
	width: 300
	height: 220
}
