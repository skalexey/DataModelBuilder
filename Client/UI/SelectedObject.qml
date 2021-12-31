import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 500
    height: 1080

    Text {
        x: 163
        y: 75
        text: selectedObjectName
        font.pixelSize: 30
    }

    ListView {
        id: params
        x: 195
        y: 149
        width: 110
        height: 160
        delegate: SelectedObjectParam {}
        model: selectedObjParamListModel
        highlight: Rectangle {
            property var currentItem: params.currentItem
            color: "yellow"
            width: parent ? parent.width : 0
            height: currentItem ? currentItem.height : 0
            y: currentItem ? currentItem.y : 0
        }
        highlightFollowsCurrentItem: false
        footer: ParamListFooter {}
    }
}
