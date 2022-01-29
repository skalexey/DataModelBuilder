import QtQuick
import QtQuick.Controls

Item {
    id: item1
    width: 500
    height: parent.height
    state: "selectedType"

	function stateItem() {
		state = "selectedItem";
		// Force selectedItem state even if the visibility has not changed
		selectedItem.changeState(getSelectedItemState());
	}

	function stateType() {
		state = "selectedType";
	}

    SelectedItem {
        id: selectedItem
        width: parent.width
        anchors.fill: parent
    }

    SelectedObject {
        id: selectedObject
        width: parent.width
        anchors.fill: parent
    }
    states: [
        State {
            name: "selectedType"

            PropertyChanges {
                target: selectedItem
                visible: false
            }
        },
        State {
            name: "selectedItem"

            PropertyChanges {
                target: selectedObject
                visible: false
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}D{i:1}D{i:2}
}
##^##*/
