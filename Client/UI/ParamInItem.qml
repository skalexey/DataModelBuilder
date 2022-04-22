import QtQuick
import QtQuick.Controls
import QVL 1.0

InteractiveListElement {
	id: obj
	width: parent.width
	height: paramValue.height
	state: "base"

	function isList() {
		return getParentModel().type === ObjectProperty.List;
	}

	property var objectMenuModel: ListModel {
		ListElement {
			title: "Rename"
			cmd: function(i) {
				obj.state = "editName";
				idInput.edit(getName());
			}
		}
		ListElement {
			title: "Edit Value"
			cmd: function(i) {
				paramValue.state = "textEdit";
				paramValue.valueInput.edit(getValueStr());
			}
		}
		ListElement {
			title: "Edit Type"
			cmd: function(i) {
				obj.state = "editType";
				paramTypeInput.currentIndex = getType()
			}
		}
		ListElement {
			title: "Delete"
			cmd: function(i) {
				getParentModel().removeAt(index);
			}
		}
	}

	property var listMenuModel: ListModel {
		ListElement {
			title: "Edit Value"
			cmd: function(i) {
				paramValue.state = "textEdit";
				paramValue.valueInput.edit(getValue());
			}
		}
		ListElement {
			title: "Edit Type"
			cmd: function(i) {
				obj.state = "editType";
				paramTypeInput.currentIndex = getType()
			}
		}
		ListElement {
			title: "Delete"
			cmd: function(i) {
				getParentModel().removeAt(index);
			}
		}
	}

	menuModel: isList() ? listMenuModel: objectMenuModel;

	Item {
		id: infoBox
		width: parent.width
		height: 40

		Text {
			id: paramId
			x: 6
			y: 12
			text: getName()
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			anchors.leftMargin: 6
			font.bold: true
			width: 200
			clip: true
			visible: !isList()
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

		Text {
			id: indexText
			x: 27
			y: 12
			text: "[" + index + "]"
			font.pixelSize: 12
			visible: isList()
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
				setType(currentValue);
				obj.state = "base" // Exit from edit state
				paramValue.chooseState();
			}
			Component.onCompleted: {
				currentIndex = getType();
			}
		}

		EditField {
			id: idInput
			textToEdit: paramId
			visible: false
			onDefocus: function() {
				obj.state = "base"
			}
			onNewValue: function(val) {
				setName(val);
			}
		}
	}

	ParamValue {
		id: paramValue
		getName: obj.getName
		getValue: obj.getValue
		getType: obj.getType
		getTypeStr: obj.getTypeStr
		getValueStr: obj.getValueStr
		setName: obj.setName
		setValue: obj.setValue
		setType: obj.setType
		getParentModel: obj.getParentModel
	}
	Text {
		id: protoName
		x: 56
		y: 12
		width: 92
		height: 16
		visible: getName() === "proto"
		text: getValueStr()
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
