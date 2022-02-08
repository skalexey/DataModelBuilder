import QtQuick
import QtQuick.Controls

InteractiveListElement {
	id: obj
	width: parent.width
	height: paramValue.height
	state: "base"

	property var textMenuModel: ListModel {
		ListElement {
			title: "Edit Value"
			cmd: function(i) {
				paramValue.state = "textEdit";
				paramValue.valueInput.edit(getValue());
			}
		}
		ListElement {
			title: "Remove"
			cmd: function(i) {
				parentModel.removeAt(index);
			}
		}
	}

	property var objectMenuModel: ListModel {
		ListElement {
			title: "Remove"
			cmd: function(i) {
				parentModel.removeAt(index);
			}
		}
	}
	menuModel: paramValue.getState() === "text" ? textMenuModel : objectMenuModel

	Item {
		id: infoBox
		width: parent.width
		height: 40

		Text {
			id: indexText
			x: 27
			y: 12
			text: "[" + index + "]"
			font.pixelSize: 12
		}

		Text {
			id: paramType
			x: 154
			y: 12
			text: getTypeStr()
			anchors.verticalCenter: parent.verticalCenter
			font.pixelSize: 12
			anchors.horizontalCenter: parent.horizontalCenter
		}
	}

	ParamValue {
		id: paramValue
		getValue: obj.getValue
		getType: obj.getType
		getTypeStr: obj.getTypeStr
		getValueStr: obj.getValueStr
		setValue: obj.setValue
		setType: obj.setType
	}

	states: [
		State {
			name: "base"
		},
		State {
			name: "edit"
		}
	]
}
