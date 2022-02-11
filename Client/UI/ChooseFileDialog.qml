import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

FileDialog {
	property var show: function() {
		visible = true;
	}

	id: fd
	title: "Choose a file"
	nameFilters: "*.json"
	onAccepted: function() {
		onOpenFile(fd.currentFile);
	}
	onRejected: {
	}
}
