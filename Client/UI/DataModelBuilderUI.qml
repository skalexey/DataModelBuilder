import QtQuick
import DataModelBuilderUI 1.0
import QtQuick.Controls
import QtQuick.Window

Item {
    id: root
    width: Constants.width
    height: Constants.height
// ======= The code for Window =======
//    visible: true
// Uncomment if you deside to test dialogs in the Qt Designer
// ======= End code for Window =======
    // ======= Aliases =======
    property alias preInstantiateDialog: preInstantiateDialog

    // ======= Selected item properties =======
	property var varListFooterAddClicked: function(listModel) {
		console.log("varListFooterAddClicked default handler");
	}

	property var getSelectedItemState: function() {
		return "object"
	}

	property var getSelectedItemPropListBlockState: function() {
		return "object"
	}

	property var selectedItemSimpleValue: "Test selected item Value"
    property string selectedItemName: "Selected item"
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
        console.log("DataModelBuilderUI: contentDeleteItemClicked(" + i + ") default handler")
    }

    property var itemClick: function() {
		screen.centralBlock.stateItem();
    }
    property var instantiateClick: function(i) { console.log("DataModelBuilderUI: instantiateClick(" + i + ") default handler"); }
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
		screen.centralBlock.stateType()
    }
    property int libraryCurrentObjectIndex: objectsLibrary.libraryContainer.currentIndex
    property var libraryAddNewObjectClicked: function() { console.log("Default click handler") }
    property var libraryDeleteObjectClicked: function(i) { console.log("DataModelBuilderUI: onDeleteObjectClicked(" + i + ") default handler") }
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
    property var selectedObjParamTypeModel: ["No modifier", "Shift", "Control"]
    property string selectedObjectName: "Obj name"
    property var selectedObjectNewParamClicked: function() { console.log("DataModelBuilderUI: onNewParamClicked default handler") }
    property var selectedObjectDeleteParameterClicked: function(i) { console.log("DataModelBuilderUI: onDeleteParameterClicked(" + i + ") Delete param default handler") }
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
    property var storeClicked: function() { console.log("DataModelBuilderUI: btnStore.onClicked default handler"); }
    // ======= End of Types properties =======

    // ======= Screen definition =======
    Screen01 {
        id: screen
        width: root.width
        height: root.height
        property alias centralBlock: centralBlock
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
        }

        ObjectsLibrary {
            id: objectsLibrary
            width: parent.width * 0.2
            height: parent.height - topBlock.height - topBlock.y
            anchors.left: parent.left
            y: topBlock.y + topBlock.height
        }

        CentralBlock {
            id: centralBlock
            width: parent.width * 0.6
            height: parent.height - topBlock.height - topBlock.y
            y: topBlock.y + topBlock.height
            anchors.horizontalCenter: parent.horizontalCenter
        }

        ContentBlock {
            id: content
            width: parent.width * 0.2
            height: parent.height - topBlock.height - topBlock.y
            anchors.right: parent.right
            y: topBlock.y + topBlock.height
        }

        PreInstantiateDialog {
            id: preInstantiateDialog
        }
    }
    // ======= End of Screen definition =======
}




