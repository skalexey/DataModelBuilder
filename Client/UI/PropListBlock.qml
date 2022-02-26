import QtQuick
import QtQuick.Controls

Column {
	id: props
	width: parent.width
	height: protoPropsLabel.height + ownPropsLabel.height + protoPropList.height + ownPropList.height + arrayContentList.height
	state: "object"

	property var ownPropListModel
	property var protoPropListModel
	property var arrayListModel
	property var changeState: function(s) {
		state = s;
	}

	Text {
		id: emptyLabel
		visible: false
		text: qsTr("<Empty>")
		font.pixelSize: 9
		anchors.left: parent.left
		anchors.leftMargin: 6
	}

	Text {
		id: ownPropsLabel
		text: qsTr("own props")
		anchors.left: parent.left
		font.pixelSize: 8
		anchors.leftMargin: 6
		visible: false
	}
	Text {
		id: ownNoPropsLabel
		visible: false
		text: qsTr("<Nothing>")
		font.pixelSize: 9
		anchors.left: parent.left
		anchors.leftMargin: 6

	}

	InteractiveListView {
		id: ownPropList
		width: parent.width
		visible: false
		implicitHeight: contentItem.childrenRect.height
		delegate: ParamInItem {
			container: ownPropList
			getName: function() { return name; }
			setName: function(val) { name = val; }
			getValue: function() { return value; }
			setValue: function(val) { value = val; }
			getValueStr: function() { return valueStr; }
			getParentModel: function() { return parentModel; }
			getType: function() { return type; }
			setType: function(val) { type = val; }
			getTypeStr: function() { return typeStr; }

			width: ownPropList.width
		}
		model: ownPropListModel
		footer: VarListFooter {
			listModel: ownPropList.model
		}
	}

	Text {
		id: protoPropsLabel
		text: qsTr("proto props")
		anchors.left: parent.left
		font.pixelSize: 8
		anchors.leftMargin: 6
		visible: false
	}

	Rectangle {
		id: protoPropListBg
		color: "#67dfffcd"
		width: parent.width
		visible: false
		implicitHeight: childrenRect.height
		InteractiveListView {
			id: protoPropList
			width: parent.width
			implicitHeight: contentItem.childrenRect.height
			delegate: ParamInItem {
				container: protoPropList
				getName: function() { return name; }
				setName: function(val) { name = val; }
				getValue: function() { return value; }
				setValue: function(val) { value = val; }
				getValueStr: function() { return valueStr; }
				getType: function() { return type; }
				setType: function(val) { type = val; }
				getTypeStr: function() { return typeStr; }
				getParentModel: function() { return parentModel; }

				width: protoPropList.width
			}
			model: protoPropListModel
			footer: VarListFooter {
				listModel: protoPropList.model
			}
		}
	}
	Rectangle {
		id: arrayListBg
		color: "#67dfffcd"
		width: parent.width
		visible: false
		implicitHeight: childrenRect.height
		InteractiveListView {
			id: arrayContentList
			width: parent.width
			implicitHeight: contentItem.childrenRect.height
			delegate: VLListElement {
				container: arrayContentList
				width: ownPropList.width
				getValue: function() { return value; }
				setValue: function(val) { value = val; }
				getValueStr: function() { return valueStr; }
				getType: function() { return type; }
				setType: function(val) { type = val; }
				getTypeStr: function() { return typeStr; }
				getParentModel: function() { return parentModel; }
			}
			model: arrayListModel
			footer: VarListFooter {
				listModel: arrayContentList.model
			}
		}
	}

	states: [
		State {
			name: "object"

			PropertyChanges {
				target: ownPropsLabel
				visible: (ownPropListModel && ownPropListModel.rowCount() > 0)
						 || (protoPropListModel && protoPropListModel.rowCount() > 0)
			}

			PropertyChanges {
				target: ownNoPropsLabel
				visible: ownPropListModel && ownPropListModel.rowCount() === 0
						 && protoPropListModel && protoPropListModel.rowCount() > 0
			}

			PropertyChanges {
				target: ownPropList
				visible: true
			}

			PropertyChanges {
				target: protoPropsLabel
				visible: protoPropListModel && protoPropListModel.rowCount() > 0
			}

			PropertyChanges {
				target: emptyLabel
				visible: (!protoPropListModel || (protoPropListModel && protoPropListModel.rowCount() === 0))
						 && (!ownPropListModel || (ownPropListModel && ownPropListModel.rowCount() === 0))
			}

			PropertyChanges {
				target: protoPropListBg
				visible: protoPropListModel && protoPropListModel.rowCount() > 0
			}
		},
		State {
			name: "list"

			PropertyChanges {
				target: arrayListBg
				color: "#67cdd0ff"
				visible: true
			}

			PropertyChanges {
				target: emptyLabel
				visible: arrayListModel && arrayListModel.rowCount() === 0
			}
		}
	]


}

/*##^##
Designer {
	D{i:0;formeditorZoom:1.5;height:22;width:216}D{i:1}D{i:2}D{i:3}D{i:6}D{i:8}D{i:7}
D{i:12}D{i:11}
}
##^##*/
