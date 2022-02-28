import QtQuick
import QtQuick.Controls

DialogTemplate {
	id: dialogTemplate
	title: ""

	property var show: function(title, message, onOk, onCancel, onNo) {
		this.title = title;
		this.message = message;
		if (onOk)
			this.onOk = onOk;
		if (onNo)
		{
			this.onNo = onNo;
			threeBtn = true;
		}
		if (onCancel)
			this.onCancel = onCancel;
		else if (!onNo)
			oneBtn = true;
		dialogTemplate_show();
	}

	property alias message: messageText.text

	Text {
		id: messageText
		anchors.fill: bodyBlock
		wrapMode: Text.WordWrap
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		anchors.verticalCenter: parent.verticalCenter
		anchors.horizontalCenter: parent.horizontalCenter
	}

	onClosed: function() {
		oneBtn = false;
		threeBtn = false;
		dialogTemplate_onClosed();
	}
}
