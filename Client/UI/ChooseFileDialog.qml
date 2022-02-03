import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

FileDialog {
	id: fd
	title: "Choose a file"
	nameFilters: "*.json"
	onAccepted: function() {
		onOpenFile(fd.currentFile);
	}
	onRejected: {
	}
}
