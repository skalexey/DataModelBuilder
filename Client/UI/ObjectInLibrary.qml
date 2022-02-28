import QtQuick
import QtQuick.Controls

InteractiveListElement {
    id: obj
    x: 0
    height: 50
    width: parent.width
    state: "base"
	onPress: function() {
		typeClick();
	}

	menuModel: ListModel {
		ListElement {
			title: "Instantiate"
			cmd: function(i) {
				libraryTypeListModel.parent.instantiate(getName());
			}
		}
		ListElement {
			title: "Copy"
			cmd: function(i) {
				var c = value.copy();
				var parentModel = libraryTypeListModel.parent;
				parentModel.set(parentModel.freeId(name), c);
			}
		}
		ListElement {
			title: "Rename"
			cmd: function(i) {
				obj.state = "edit";
				idInput.edit(getName());
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
        leftPadding: 6
        id: nameText
        text: name
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
    }

	EditField {
		id: idInput
		textToEdit: nameText
        visible: false
        text: name
		onDefocus: function() {
			obj.state = "base"
		}
		onNewValue: function(val) {
			setName(val);
		}
    }

    states: [
        State {
            name: "edit"

            PropertyChanges {
				target: idInput
                visible: true
            }

            PropertyChanges {
                target: nameText
                visible: false
            }
        },
        State {
            name: "base"
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}D{i:1}D{i:2}D{i:4}D{i:3}
}
##^##*/
