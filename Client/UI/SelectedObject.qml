import QtQuick
import QtQuick.Controls

Item {
    id: item1
    width: parent.width
    height: parent.height

    Text {
        id: selectedObjectNameText
        text: selectedObjectName
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        height: 50
    }

	InteractiveListView {
        id: params
        y: selectedObjectNameText.y + selectedObjectNameText.height
        width: parent.width * 0.5
        height: parent.height - selectedObjectNameText.height
        anchors.horizontalCenter: parent.horizontalCenter
        delegate: SelectedObjectParam {
            width: params.width // To fix the error "TypeError: Cannot read property 'width' of null"
        }
        model: selectedObjParamListModel
        footer: ParamListFooter {}
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}D{i:1}D{i:2}
}
##^##*/
