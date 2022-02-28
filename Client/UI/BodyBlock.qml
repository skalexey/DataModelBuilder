import QtQuick
import QtQuick.Controls

Item {
	id: bodyBlock
	y: topBlock.y + topBlock.height
	width: parent.width
	height: parent.height - topBlock.height - topBlock.y
	state: "chooseFile"

	property alias centralBlock: centralBlock
	property alias objectsLibrary: objectsLibrary
	property alias content: content
	property var stateOpened: function() {
		state = "fileOpened";
	}
	property var stateChooseFile: function() {
		state = "chooseFile";
	}

	Item {
		id: fileControls
		width: parent.width
		height: parent.height

		ObjectsLibrary {
			id: objectsLibrary
			x: 0
			y: 0
			width: parent.width * 0.2
			height: parent.height - topBlock.height - topBlock.y
			anchors.left: parent.left
		}

		CentralBlock {
			id: centralBlock
			x: 72
			y: 0
			width: parent.width * 0.6
			height: parent.height - topBlock.height - topBlock.y
			anchors.horizontalCenter: parent.horizontalCenter
		}

		ContentBlock {
			id: content
			x: 288
			y: 0
			width: parent.width * 0.2
			height: parent.height - topBlock.height - topBlock.y
			anchors.right: parent.right
		}
	}

	Item {
		id: chooseFileBlock
		width: parent.width
		height: parent.height
		Button {
			text: "Open file"
			anchors.verticalCenter: parent.verticalCenter
			anchors.horizontalCenter: parent.horizontalCenter
			onClicked: function() {
				openFileClicked();
			}
		}

		Column {
			id: recentFiles
			width: 200
			height: parent.height

			Text {
				id: recentFilesTitle
				text: qsTr("Text")
				font.pixelSize: 12
			}

			Text {
				id: emptyListText
				visible: !recentFilesList.model || recentFilesList.model.rowCount() === 0
				text: "No files have been opened recently"
				font.pixelSize: 12
				textFormat: Text.RichText
			}

			InteractiveListView {
				id: recentFilesList
				width: parent.width
				height: parent.height
				model: recentFilesModel
				delegate: InteractiveListElement {
					id: o
					height: 50
					width: recentFilesList.width
					container: recentFilesList
					onClick: function() {
						console.log("recentFileClicked");
						onOpenFile(value);
					}
					Text {
						leftPadding: 6
						id: nameText
						text: recentFileRelPath(value)
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: parent.left
						font.bold: true
						anchors.horizontalCenter: parent.horizontalCenter
					}
				}
			}
		}
	}

	states: [
		State {
			name: "fileOpened"

			PropertyChanges {
				target: chooseFileBlock
				visible: false
			}
		},
		State {
			name: "chooseFile"

			PropertyChanges {
				target: fileControls
				width: parent.width
				visible: false
			}

			PropertyChanges {
				target: recentFilesTitle
				text: "Recent files"
				font.pixelSize: 24
			}

			PropertyChanges {
				target: recentFiles
				height: parent.height
			}
		}
	]
}

/*##^##
Designer {
	D{i:0;formeditorZoom:0.66}D{i:2}D{i:3}D{i:4}D{i:1}D{i:6}D{i:8}D{i:9}D{i:10}D{i:7}
D{i:5}
}
##^##*/
