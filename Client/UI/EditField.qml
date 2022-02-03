import QtQuick
import QtQuick.Controls

TextField {
	property var textToEdit: null
	property var onDefocus: function() {
		console.log("EditField.onDefocus default handler");
	}
	property var onNewValue: function(val) {
		console.log("EditField.onNewValue(" + val + ") default handler");
	}
	property var edit: function(value) {
		forceActiveFocus();
		text = value;
	}

	QtObject {
		id: local
		property bool ignoreChanges: false;
	}
	id: idInput
	width: Math.max(100, textToEdit.width * 1.4)
	height: textToEdit.height * 1.4
	x: textToEdit.x
	y: textToEdit.y
	placeholderText: qsTr("Enter text")
	Keys.onReturnPressed: focus = false;
	Keys.onEnterPressed: focus = false;
	Keys.onEscapePressed: function() {
		local.ignoreChanges = true;
		focus = false;
		local.ignoreChanges = false;
	}
	onFocusChanged: function() {
		if (!focus)
		{
			if (!local.ignoreChanges)
				onNewValue(text);
			onDefocus();
		}
	}
}
