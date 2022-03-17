import QtQuick
import QtQuick.Controls

DialogTemplate {
	id: dialog
	onShow: function() {
		textInput.forceActiveFocus();
	}

	onOk: function() {
		onConfirm(textInput.text);
	}

	function log(msg) {
		console.log("EnterTextLineDialog: " + msg);
	}

	property alias initialText: textInput.text
	property alias placeholderText: textInput.placeholderText


	property var onConfirm: function(enteredText) {
		log("onConfirm: default handler")
	}

	title: "Enter a text"

	TextField {
		id: textInput
		placeholderText: "Enter text"
		Keys.onReturnPressed: {
			dialog.onConfirm(text);
			dialog.close();
		}
		Keys.onEnterPressed: Keys.onReturnPressed
	}
}
