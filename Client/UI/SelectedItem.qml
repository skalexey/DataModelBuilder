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
		propListBlock.arrayListModel = selectedItemArrayListModel
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
		visible: false
		protoPropListModel: selectedItemProtoParamListModel
		ownPropListModel: selectedItemOwnParamListModel
		arrayListModel: selectedItemArrayListModel
		width: parent.width * 0.6
		anchors.horizontalCenter: parent.horizontalCenter
	}

	ParamInItem {
		id: itemValue
		visible: false
		getName: function() { return selectedItemName; }
		setName: function(val) { selectedItemChangeName(val); }
		getValue: function() { return selectedItemValue(); }
		setValue: function(val) { selectedItemChangeValue(val); }
		getValueStr: function() { return selectedItemSimpleValue(); }
		getType: function() { return getSelectedItemType(); }
		setType: function(val) { setSelectedItemType(val); }
		getTypeStr: function() { return getSelectedItemTypeStr(); }
		getParentModel: function() { return getSelectedItemParentModel(); }
		width: parent.width
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
				target: itemValue
				visible: true
			}
		},
		State {
			name: "object"
			PropertyChanges {
				target: propListBlock
				visible: true
			}
		},
		State {
			name: "list"
			PropertyChanges {
				target: propListBlock
				visible: true
			}
		}
	]
}


