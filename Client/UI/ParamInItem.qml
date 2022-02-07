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
				obj.state = "editName";
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
			title: "Edit Type"
			cmd: function(i) {
				console.log("paramValue.state: " + paramValue.state)
				obj.state = "editType";
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
			width: 200
			clip: true
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

		ComboBox {
			id: paramTypeInput
			x: paramType.x
			y: paramType.y
			height: paramType.height * 1.4
			width: 103
			visible: false
			model: typesListsModel
			onActivated: {
				type = currentValue;
				console.log("paramValue.state: " + paramValue.state)
				obj.state = "base";
				paramValue.chooseState();
			}
			Component.onCompleted: currentIndex = type
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
			name: "editName"

			PropertyChanges {
				target: idInput
				visible: true
			}

			PropertyChanges {
				target: paramId
				visible: false
			}
		},
		State {
			name: "editType"
			PropertyChanges {
				target: idInput
				visible: false
			}

			PropertyChanges {
				target: paramId
				visible: true
			}

			PropertyChanges {
				target: paramTypeInput
				visible: true
			}

			PropertyChanges {
				target: paramType
				visible: false
			}
		}
	]
}

/*##^##
Designer {
	D{i:0;formeditorZoom:1.25}D{i:7}D{i:8}D{i:9}D{i:10}D{i:6}D{i:11}D{i:12}
}
##^##*/
