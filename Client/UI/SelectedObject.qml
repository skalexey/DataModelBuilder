import QtQuick
import QtQuick.Controls

Column {
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

	PropListBlock {
		id: propListBlock
		protoPropListModel: selectedObjProtoPropListModel
		ownPropListModel: selectedObjParamListModel
		width: parent.width * 0.6
		anchors.horizontalCenter: parent.horizontalCenter
	}
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}D{i:1}D{i:2}
}
##^##*/
