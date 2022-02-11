import QtQuick
import QtQuick.Controls

Item {
	id: item1
	width: 350
	height: 1080

	property alias contentContainer: contentBlockContainer
	Text {
		id: contentBlockTitle
		text: qsTr("Content")
		font.pixelSize: 24
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
	}

	InteractiveListView {
		id: contentBlockContainer
		y: contentBlockTitle.y + contentBlockTitle.height
		width: parent.width * 0.8
		height: parent.height - contentBlockTitle.y - contentBlockTitle.height
		anchors.horizontalCenter: parent.horizontalCenter
		orientation: ListView.Vertical
		model: contentListModel
		delegate: ItemInContent {
			width: contentBlockContainer.width // To fix the error "TypeError: Cannot read property 'width' of null"
			container: contentBlockContainer
			getName: function() { return name; }
			setName: function(val) { name = val; }
			getValue: function() { return value; }
			setValue: function(val) { value = val; }
			getValueStr: function() { return valueStr; }
			getType: function() { return type; }
			setType: function(val) { type = val; }
			getTypeStr: function() { return typeStr; }
		}
		footer: VarListFooter {
			listModel: contentListModel
			buttonText: "Add a constant"

			Item {
				id: spacer
				height: 12
				width: parent.width
			}
		}
	}
}
