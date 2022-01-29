import QtQuick
import QtQuick.Controls

InteractiveListElement {
	id: obj
	width: parent.width
	height: paramValue.height
	state: "base"

	menuModel: ListModel {
		ListElement {
			title: "Rename"
			cmd: function(i) {
				obj.state = "edit";
				idInput.edit(name);
			}
		}
		ListElement {
			title: "Edit Value"
			cmd: function(i) {
				paramValue.state = "textEdit";
				paramValue.valueInput.edit(value);
			}
		}
		ListElement {
			title: "Delete"
			cmd: function(i) {
				parentModel.removeAt(index);
			}
		}
	}

	Item {
		id: infoBox
		width: parent.width
		height: 40

		Text {
			id: paramId
			x: 6
			y: 12
			text: name
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			anchors.leftMargin: 6
			font.bold: true
		}

		Text {
			id: paramType
			x: 154
			y: 12
			text: typeStr
			anchors.verticalCenter: parent.verticalCenter
			font.pixelSize: 12
			anchors.horizontalCenter: parent.horizontalCenter
		}

		EditField {
			id: idInput
			textToEdit: paramId
			visible: false
			onDefocus: function() {
				obj.state = "base"
			}
			onNewValue: function(val) {
				name = val
			}
		}
	}

	ParamValue {
		id: paramValue
		propVal: value // RoleValue
	}
	Text {
		id: protoName
		x: 56
		y: 12
		width: 92
		height: 16
		visible: name === "proto"
		text: valueStr
		font.pixelSize: 12
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		transformOrigin: Item.Center
		maximumLineCount: 1
		style: Text.Raised
		styleColor: "#302bff"
	}

	states: [
		State {
			name: "base"
		},
		State {
			name: "edit"

			PropertyChanges {
				target: idInput
				visible: true
			}

			PropertyChanges {
				target: paramId
				visible: false
			}
		}
	]
}
