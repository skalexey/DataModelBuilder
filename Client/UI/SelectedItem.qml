import QtQuick
import QtQuick.Controls

Column {
	id: item1
    width: parent.width
	height: parent.height
	state: "object"

	property alias propListBlock: propListBlock
	property var changeState: function(s) {
		state = s;
		propListBlock.changeState(getSelectedItemPropListBlockState());
	}

    Text {
        id: selectedItemNameText
        width: parent.width
        height: 40
        text: selectedItemName
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

	PropListBlock {
		id: propListBlock
		protoPropListModel: selectedItemProtoParamListModel
		ownPropListModel: selectedItemOwnParamListModel
		arrayListModel: selectedItemArrayListModel
		width: parent.width * 0.6
		anchors.horizontalCenter: parent.horizontalCenter
	}

	Text {
		id: itemValue
		text: selectedItemSimpleValue
		font.pixelSize: 12
	}

	// TODO: support scrollbar
	ScrollBar {
			id: vbar
			hoverEnabled: true
			active: hovered || pressed
			orientation: Qt.Vertical
			size: item1.height / propListBlock.height
//			anchors.top: parent.top
//			anchors.right: parent.right
//			anchors.bottom: parent.bottom
		}
	states: [
		State {
			name: "simpleType"
			PropertyChanges {
				target: propListBlock
				visible: false
			}
		},
		State {
			name: "object"

			PropertyChanges {
				target: itemValue
				visible: false
			}
		},
		State {
			name: "list"

			PropertyChanges {
				target: itemValue
				visible: false
			}
		}
	]
}


