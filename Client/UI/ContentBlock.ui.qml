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
		}
		footer: ContentListFooter {}
	}
}
