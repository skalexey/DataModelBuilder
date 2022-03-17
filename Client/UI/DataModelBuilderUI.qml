import QtQuick
import DataModelBuilderUI 1.0
import QtQuick.Controls
import QtQuick.Window

Item {
	id: root
	width: Constants.width
	height: Constants.height

	function log(msg) {
		console.log("DataModelBuilderUI: " + msg);
	}

	// ======= The code for Window =======
	//    visible: true
	// Uncomment if you deside to test dialogs in the Qt Designer
	// ======= End code for Window =======
	// ======= Aliases =======
	property alias preInstantiateDialog: preInstantiateDialog
	property alias chooseFileDialog: chooseFileDialog
	property alias saveAsDialog: saveAsDialog
	property alias dialogMessage: dialogMessage
	property alias bodyBlock: bodyBlock
	// ======= Types list models
	property var typesListsModel: ["No modifier", "Shift", "Control"]
	property var typesListsModelDetailed: [{"No modifier": 0}, {"Shift": 1}, {"Control": 2}]

	// ======= Files properties ========
	property var textCurrentFile: "No file opened"
	// ======= Recent files properties ========
	property var recentFileRelPath: function(fPath) {
		// Default handler. Will be overriden in main.qml
		var i = fPath.lastIndexOf("/");
		if (i >= 0)
			return fPath.substring(i + 1);
		return fPath;
	}

	property var recentFilesModel: ListModel {

	}

	// ======= Selected item properties =======
	property var varListFooterAddClicked: function(listModel) {
		log("varListFooterAddClicked default handler");
	}

	property var getSelectedItemState: function() {
		return "object"
	}

	property var getSelectedItemPropListBlockState: function() {
		return "object"
	}

	property var selectedItemSimpleValue: function () { return "Test selected item valueStr"; }
	property var selectedItemValue: function() { return "Test selectedItemValue" };

	property var selectedItemChangeValue: function(val) {
		log("selectedItemChangeValue(" + val + ") default handler");
	}
	property var selectedItemChangeName: function(val) {
		log("selectedItemChangeName(" + val + ") default handler");
	}
	property var setSelectedItemType: function(val) {
		log("setSelectedItemType(" + val + ") default handler");
	}
	property var getSelectedItemType: function() {
		log("getSelectedItemType() default handler");
	}
	property var getSelectedItemTypeStr: function() {
		log("getSelectedItemTypeStr() default handler");
	}
	property var getSelectedItemParentModel: function() {
		log("getSelectedItemParentModel() default handler");
	}

	property string selectedItemName: "Selected item"
	QtObject {
		id: local
		property var selectedItemArrayListModel: ListModel {
			ListElement {
				name: "Element 1"
				value: "e1"
			}

			ListElement {
				name: "Element 2"
				value: "b2"
			}

			ListElement {
				name: "Element 3"
				value: "a3"
			}

			ListElement {
				name: "Element 4"
				value: "d4"
			}
		}
	}

	property var selectedItemArrayListModel: function() {
		return local.selectedItemArrayListModel;
	}
	property var selectedItemOwnParamListModel: ListModel {
		ListElement {
			name: "Grey"
			value: "grey"
		}

		ListElement {
			name: "Red"
			value: "red" // {"color": "red", "pale": false, "parent": {}}
		}

		ListElement {
			name: "Blue"
			value: "blue"
		}

		ListElement {
			name: "Green"
			value: "green"
		}
	}
	property var selectedItemProtoParamListModel: ListModel {
		ListElement {
			name: "Grey"
			value: "grey"
		}

		ListElement {
			name: "Red"
			value: "red" // {"color": "red", "pale": false, "parent": {}}
		}

		ListElement {
			name: "Blue"
			value: "blue"
		}

		ListElement {
			name: "Green"
			value: "green"
		}
	}
	// ======= End of Selected item properties =======

	// ======= Content properties =======
	property int contentCurrentItemIndex: content.contentContainer.currentIndex
	property var contentDeleteItemClicked: function(i) {
		log("DataModelBuilderUI: contentDeleteItemClicked(" + i + ") default handler")
	}

	property var itemClick: function() {
		screen.centralBlock.stateItem();
	}
	property var connectInstantiation: function(model) {
		model.onInstantiateRequested.connect(function(instId, protoId) {
			preInstantiateDialog.show();
			preInstantiateDialog.initialText = instId
			preInstantiateDialog.onConfirm = function(enteredText) {
				log("Instantiate '" + enteredText + "' of type '" + protoId + "'");
				model.instantiate(protoId, enteredText);
			}
		});
		model.onInstantiateRefused.connect(function(error) {
			log("Instantiate refused: " + error);
		});
	}
	property var contentListModel: ListModel {
		ListElement {
			name: "Grey"
		}

		ListElement {
			name: "Red"
		}

		ListElement {
			name: "Blue"
		}

		ListElement {
			name: "Green"
		}

		ListElement {
			name: "Purple"
		}
	}
	// ======= End of Content properties =======

	// ======= Types properties =======
	property var typeClick: function() {
		screen.centralBlock.stateType();
	}
	property alias objectsLibrary: bodyBlock.objectsLibrary
	property alias content: bodyBlock.content
	property int libraryCurrentObjectIndex: objectsLibrary.libraryContainer.currentIndex
	property var libraryAddNewObjectClicked: function() { log("Default click handler") }
	property var libraryDeleteObjectClicked: function(i) { log("DataModelBuilderUI: onDeleteObjectClicked(" + i + ") default handler") }
	property var libraryTypeListModel: ListModel {
		ListElement {
			name: "Scheme element"
		}

		ListElement {
			name: "Connection"
		}

		ListElement {
			name: "Back image"
		}

		ListElement {
			name: "Square"
		}
	}
	property string selectedObjectName: "Obj name"
	property var selectedObjectNewParamClicked: function() { log("DataModelBuilderUI: onNewParamClicked default handler") }
	property var selectedObjectDeleteParameterClicked: function(i) { log("DataModelBuilderUI: onDeleteParameterClicked(" + i + ") Delete param default handler") }
	property var selectedObjParamListModel: ListModel {
		ListElement {
			name: "Type 1"
		}

		ListElement {
			name: "Type 2"
		}

		ListElement {
			name: "Type 3"
		}
	}
	property var selectedObjProtoPropListModel: ListModel {
		ListElement {
			name: "Type 1"
		}

		ListElement {
			name: "Type 2"
		}

		ListElement {
			name: "Type 3"
		}
	}
	property var storeClicked: function() { log("DataModelBuilderUI: btnStore.onClicked default handler"); }
	// ======= End of Types properties =======

	// ======= Screen properties =======
	property var openFileClicked: function() { log("openFileClicked default handler"); }
	property var saveAsClicked: function() { log("saveAsClicked default handler"); }
	property var saveClicked: function() { log("saveClicked default handler"); }
	property var onOpenFile: function(fPath) { log("onOpenFile default handler"); }
	property var newClicked: function() { log("newClicked default handler"); }
	property var closeFileClicked: function() { log("closeFileClicked default handler"); }
	property var exportContentClicked: function() { log("exportContentClicked default handler"); }

	// ======= Screen definition =======
	Screen01 {
		id: screen
		width: root.width
		height: root.height
		property alias centralBlock: bodyBlock.centralBlock
		Rectangle {
			id: topBlock
			width: parent.width
			height: 60
			Button {
				id: btnStore
				x: 14
				text: qsTr("Store")
				anchors.verticalCenter: parent.verticalCenter
				onClicked: storeClicked()
			}
			Button {
				id: btnFile
				x: 14
				text: qsTr("File")
				anchors.verticalCenter: parent.verticalCenter
				onClicked: bodyBlock.isChooseFileState() ? fileMenu.popup() : currentFileMenu.popup()
			}
			Text {
				x: btnFile.x + btnFile.width + 12
				anchors.verticalCenter: parent.verticalCenter
				text: textCurrentFile
			}

			FileMenu {
				id: fileMenu
			}

			CurrentFileMenu {
				id: currentFileMenu
			}
		}

		BodyBlock {
			id: bodyBlock
		}

		PreInstantiateDialog {
			id: preInstantiateDialog
		}

		ChooseFileDialog {
			id: chooseFileDialog
		}

		SaveAsDialog {
			id: saveAsDialog
		}

		DialogMessage {
			id: dialogMessage
		}

	}
	// ======= End of Screen definition =======
}





/*##^##
Designer {
	D{i:0;formeditorZoom:0.66}D{i:33}D{i:40}D{i:41}D{i:42}D{i:43}D{i:32}
}
##^##*/
