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
	property var onConfirm: function(enteredText) {
		log("onConfirm('" + enteredText + "') default handler")
	}

	title: "Enter a text"
	onOk: function() {
		onConfirm(textInput.text);
	}
	onCancel: function() {
		log("Cancel clicked");
	}
	TextField {
		id: textInput
		placeholderText: "Enter text"
		Keys.onReturnPressed: {
			onConfirm(text);
			dialog.close();
		}
		Keys.onEnterPressed: Keys.onReturnPressed
	}
}
