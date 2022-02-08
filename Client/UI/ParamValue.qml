import QtQuick
import QtQuick.Controls

VarInterface {
	id: paramValue
	width: parent.width
	implicitHeight: childrenRect.height

	property alias valueInput: textField

	function asString(val) {
		if (val !== null)
			return typeof val == "string" ? val : val.toString();
		return "<null>"
	}

	property var getState: function(val) {
		switch (typeof val)
		{
			case "object":
			case "array":
				return "collapsed";
			case "number":
			case "string":
			case "boolean":
				return "text";
			default:
				return "none";
		}
	}

	function chooseState() {
		state = getState(getValue());
	}

	state: getState(getValue())

	Column {
		id: item1
		width: parent.width

		Item {
			id: control
			width: parent.width
			height: 40
			Text {
				id: text1
				x: 268
				visible: false
				text: getValueStr()
				anchors.verticalCenter: parent.verticalCenter
				anchors.right: parent.right
				font.pixelSize: 12
				width: 200
				clip: true
			}

			EditField {
				id: textField
				textToEdit: text1
				x: 268
				visible: false
				onDefocus: function() {
					paramValue.chooseState();
				}
				onNewValue: function(val) {
					setValue(val);
				}
			}

			CheckBox {
				id: checkBox
				x: 259
				visible: false
				text: qsTr("Check Box")
				anchors.verticalCenter: parent.verticalCenter
				anchors.right: parent.right
			}

			Switch {
				id: expandSwitch
				x: 251
				visible: false
				text: qsTr("Expand")
				anchors.verticalCenter: parent.verticalCenter
				anchors.right: parent.right
				property var addedPropsBlock: null
				onClicked: {
					if (checked)
					{
						var c = null;
						function create() {
//							console.log("Create component PropListBlock.qml");
							c = Qt.createComponent("PropListBlock.qml");
							if (c.status === Component.Ready)
								finishCreation();
							else
							{
//								console.log("Creation is not ready yet. Wait")
								c.statusChanged.connect(finishCreation);
							}
						}

						function finishCreation() {
//							console.log("finish creation")
							if (c.status === Component.Ready) {
								if (getTypeStr() === "List")
								{
									addedPropsBlock = c.createObject(item1, {
										state: "list",
										x: 10,
										arrayListModel: getValue().listModel
									});
								}
								else if (getTypeStr() === "Object")
								{
									addedPropsBlock = c.createObject(item1, {
										state: "object",
										x: 10,
										ownPropListModel: getValue().propListModel,
										protoPropListModel: getValue().protoPropListModel
									});
								}
								if (addedPropsBlock == null) {
									console.log("Error creating object");
								}
								else
								{
//									console.log("Creation is completed successfully");
									paramValue.state = "expanded"
								}
							} else if (c.status === Component.Error) {
								// Error Handling
								console.log("Error loading component:", c.errorString());
							}
						}
						create();
					}
					else
					{
						paramValue.state = "collapsed"
						if (addedPropsBlock)
						{
							addedPropsBlock.destroy()
							addedPropsBlock = null;
						}
					}
				}
			}
		}
	}


	states: [
		State {
			name: "text"
			PropertyChanges {
				target: text1
				visible: true
			}
		},
		State {
			name: "textEdit"

			PropertyChanges {
				target: textField
				visible: true
			}
		},
		State {
			name: "flag"

			PropertyChanges {
				target: checkBox
				visible: true
			}
		},
		State {
			name: "collapsed"

			PropertyChanges {
				target: checkBox
				visible: false
			}

			PropertyChanges {
				target: expandSwitch
				visible: true
				text: qsTr("Expand")
			}
		},
		State {
			name: "expanded"

			PropertyChanges {
				target: expandSwitch
				visible: true
				text: qsTr("Collapse")
			}
		}
	]
}

/*##^##
Designer {
	D{i:0;formeditorZoom:1.25}D{i:3}D{i:4}D{i:5}D{i:6}D{i:2}D{i:1}
}
##^##*/
