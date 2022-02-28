import QtQuick
import QtQuick.Controls

DialogTemplate {
	id: dialog
	onShow: function() {
		textInput.forceActiveFocus();
	}

	function log(msg) {
		console.log("EnterTextLineDialog: " + msg);
	}

	property alias initialText: textInput.text
	property alias placeholderText: textInput.placeholderText

	title: "Enter a text"

	TextField {
		id: textInput
		placeholderText: "Enter text"
		Keys.onReturnPressed: {
			dialog.onOk(text);
			dialog.close();
		}
		Keys.onEnterPressed: Keys.onReturnPressed
	}
}
