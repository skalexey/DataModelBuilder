import QtQuick
import DMBModel 1.0
import DataModelBuilderUI 1.0

Window {
    id: root
    width: Constants.width
    height: Constants.height
	visible: true
	// To be used in libraryTypeListModel.onRowsRemoved
	property var rowsRemovedF: uiRoot.onObjectRemoved

	DMBModel {
		id: dmbModel
		onInstantiateRequested: function(instId, protoId) {
			uiRoot.preInstantiateDialog.visible = true
			uiRoot.preInstantiateDialog.instanceName = instId
			uiRoot.preInstantiateDialog.onConfirm = function(enteredText) {
				console.log("Instantiate '" + enteredText + "' of type '" + protoId + "'")
				dmbModel.addToContent(enteredText, protoId)
			}

		}
		onInstantiateRefused: function(error) {
			console.log("Instantiate refused: " + error);
		}
	}

	ListView {
		id: listView
		x: 544
		y: 249
		width: 110
		height: 160
		model: dmbModel.typeModel.propListModel
		delegate: Item {
			x: 5
			width: 80
			height: 40
			Row {
				id: row1
				spacing: 10
				Rectangle {
					width: 40
					height: 40
					color: colorCode
				}

				Text {
					text: name
					anchors.verticalCenter: parent.verticalCenter
					font.bold: true
				}
			}
		}
	}


}
