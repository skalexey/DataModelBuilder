import QtQuick
import QtQuick.Controls

InteractiveListElement {
    id: obj
    width: parent.width
    height: 40
    state: "base"

	onPress: function() {
		itemClick();
	}

	menuModel: ListModel {
		ListElement {
			title: "Rename"
			cmd: function(i) {
				obj.state = "edit";
				idInput.edit(name);
			}
		}
		ListElement {
			title: "Delete"
			cmd: function(i) {
				parentModel.removeAt(index);
			}
		}
	}

    Text {
		id: idText
        text: name
        anchors.verticalCenter: parent.verticalCenter
        leftPadding: 6
        anchors.left: parent.left
        font.bold: true
    }

	EditField {
		id: idInput
		textToEdit: idText
		visible: false
        text: name
		onDefocus: function() {
			obj.state = "base"
		}
		onNewValue: function(val) {
			name = val
		}
    }

    states: [
        State {
            name: "base"
        },
        State {
            name: "edit"

            PropertyChanges {
				target: idText
                visible: false
            }

            PropertyChanges {
				target: idInput
                focus: true
				visible: true
            }
        }
    ]
}
