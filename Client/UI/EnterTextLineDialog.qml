import QtQuick
import QtQuick.Controls

DialogTemplate {
	id: dialog
	onShow: function() {
		textInput.forceActiveFocus();
	}

	property alias initialText: textInput.text
	property alias placeholderText: textInput.placeholderText
	property var onConfirm: function(enteredText) {
		console.log("EnterTextLineDialog: onConfirm('" + enteredText + "') default handler")
	}

	title: "Enter a text"
	onAccepted: onConfirm(textInput.text);
	onRejected: console.log("Cancel clicked")
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
