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

	ScrollView {
		id: scrollView
		width: parent.width
		height: parent.height
		clip: true

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
	}
	states: [
		State {
			name: "simpleType"
			PropertyChanges {
				target: scrollView
				contentHeight: itemValue.height
			}
			PropertyChanges {
				target: itemValue
				visible: true
			}
		},
		State {
			name: "object"
			PropertyChanges {
				target: scrollView
				contentHeight: propListBlock.height
			}
			PropertyChanges {
				target: propListBlock
				visible: true
			}
		},
		State {
			name: "list"
			PropertyChanges {
				target: scrollView
				contentHeight: propListBlock.height
			}
			PropertyChanges {
				target: propListBlock
				visible: true
			}
		}
	]
}


