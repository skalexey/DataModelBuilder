import QtQuick
import QtQuick.Controls

Dialog {
    property alias instanceName: instanceNameInput.text
    property var onConfirm: function(enteredText) {
        console.log("PreInstanceDialog: onConfirm('" + enteredText + "') default handler")
    }

    id: preInstantiateDialog
    title: "Instance creation"
    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: onConfirm(instanceNameInput.text);
    onRejected: console.log("Cancel clicked")
    modal: true
    x: root.width / 2 - width / 2
    y: root.height / 2 - height / 2
    width: 300
    height: 220
    TextField {
        id: instanceNameInput
    }
}
