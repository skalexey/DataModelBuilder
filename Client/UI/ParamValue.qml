import QtQuick
import QtQuick.Controls

Item {
	id: paramValue
	width: parent.width
	implicitHeight: childrenRect.height

	property var propVal: "test value"
	property alias valueInput: textField

	function asString(val) {
		if (val !== null)
			return typeof val == "string" ? val : val.toString();
		return "<null>"
	}

	property var getState: function() {
		switch (typeof propVal)
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
		state = getState(propVal);
	}

	state: getState(propVal)

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
				text: asString(propVal)
				anchors.verticalCenter: parent.verticalCenter
				anchors.right: parent.right
				font.pixelSize: 12
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
					value = val
				}
			}

			CheckBox {
				id: checkBox
				x: 259
				text: qsTr("Check Box")
				anchors.verticalCenter: parent.verticalCenter
				anchors.right: parent.right
			}

			Switch {
				id: expandSwitch
				x: 251
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
//								console.log("item1.height before adding: " + item1.height);
//								console.log("typeof propVal: " + typeof propVal);
//								console.log("propVal: '" + propVal + "'");
//								console.log("typeStr: " + typeStr);
								if (typeStr === "List")
								{
									addedPropsBlock = c.createObject(item1, {
										state: "list",
										x: 10,
										arrayListModel: propVal.listModel
									});
								}
								else if (typeStr === "Object")
								{
									addedPropsBlock = c.createObject(item1, {
										state: "object",
										x: 10,
										ownPropListModel: propVal.propListModel,
										protoPropListModel: propVal.protoPropListModel
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
				target: checkBox
				visible: false
			}

			PropertyChanges {
				target: expandSwitch
				visible: false
			}
		},
		State {
			name: "textEdit"

			PropertyChanges {
				target: checkBox
				visible: false
			}

			PropertyChanges {
				target: expandSwitch
				visible: false
			}

			PropertyChanges {
				target: text1
				visible: false
			}

			PropertyChanges {
				target: textField
				visible: true
			}
		},
		State {
			name: "flag"

			PropertyChanges {
				target: expandSwitch
				visible: false
			}
		},
		State {
			name: "collapsed"

			PropertyChanges {
				target: checkBox
				visible: false
			}

			PropertyChanges {
				target: text1
				visible: false
			}
		},
		State {
			name: "expanded"

			PropertyChanges {
				target: checkBox
				visible: false
			}

			PropertyChanges {
				target: text1
				visible: false
			}

			PropertyChanges {
				target: expandSwitch
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
