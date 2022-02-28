import QtQuick
import QtQuick.Controls

Dialog {
	id: dialogTemplate
	title: qsTr("Title")

	property bool oneBtn: false
	property bool threeBtn: false
	property alias bodyBlock: bodyBlock

	property var dialogTemplate_onShow: function() {

	}
	property var dialogTemplate_show: function() {
		if (oneBtn)
			bottomRow.stateOneButton();
		else if (threeBtn)
			bottomRow.stateThreeButtons();
		else
			bottomRow.stateTwoButtons();
		visible = true;
		if (onShow)
			onShow();
	}
	property var onShow: dialogTemplate_onShow
	property var show: dialogTemplate_show
	property string dialogTemplate_buttonOkText: qsTr("Ok")
	property string dialogTemplate_buttonNoText: qsTr("No")
	property string dialogTemplate_buttonCancelText: qsTr("Cancel")
	property string buttonOkText: dialogTemplate_buttonOkText
	property string buttonNoText: dialogTemplate_buttonNoText
	property string buttonCancelText: dialogTemplate_buttonCancelText
	property var dialogTemplate_onOk: function() {
		//console.log("dialogTemplate_onOk default handler");
		return true;
	}

	property var dialogTemplate_onNo: function() {
		//console.log("dialogTemplate_onNo default handler");
		return true;
	}

	property var dialogTemplate_onCancel: function() {
		//console.log("dialogTemplate_onCancel default handler");
	}

	property var dialogTemplate_onClosed: function() {
		buttonOkText = dialogTemplate_buttonOkText;
		buttonNoText = dialogTemplate_buttonNoText;
		buttonCancelText = dialogTemplate_buttonCancelText;
		onOk = dialogTemplate_onOk;
		onNo = dialogTemplate_onNo;
		onCancel = dialogTemplate_onCancel;
	}

	property var onOk: dialogTemplate_onOk
	property var onNo: dialogTemplate_onNo
	property var onCancel: dialogTemplate_onCancel
	onClosed: dialogTemplate_onClosed

	modal: true
	x: parent.width / 2 - width / 2
	y: parent.height / 2 - height / 2
	width: 300
	height: 220

	Item {
		id: bodyBlock
		width: parent.width
		height: parent.height - bottomRow.height
	}

	DialogBottomRow {
		id: bottomRow
		dialog: dialogTemplate
	}
}

/*##^##
Designer {
	D{i:0;formeditorZoom:0.9}D{i:2}D{i:3}D{i:1}
}
##^##*/
