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
		footer: VarListFooter {
			listModel: contentListModel
			buttonText: "Add a constant"
			Item {
				id: spacer
				height: 12
				width: parent.width
			}

			Button {
				id: instantiateType
				text: "Add a type instance"
				onClicked: {
					contextMenu.popup();
				}
			}
			Menu {
				id: contextMenu
				Instantiator {
					model: libraryTypeListModel
					onObjectAdded: function(i, o) {
						contextMenu.insertItem(i, o);
					}
					onObjectRemoved: function(o) {
						contextMenu.removeItem(o);
					}
					delegate: Component {
						MenuItem {
							text: name
							onTriggered: {
								instantiateClick(index);
							}
						}
					}
				}
			}
		}
	}
}
