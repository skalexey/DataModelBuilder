import QtQuick
import QtQuick.Controls

Item {
    width: 300
    height: 1080

    property alias libraryContainer: objectsLibraryContainer

    Text {
        id: objectsLibraryTitle
        x: 51
        y: 49
        text: qsTr("Objects Library")
        font.pixelSize: 30
    }

    ListView {
        id: objectsLibraryContainer
        x: 33
        y: 136
        width: 237
        height: 900
        delegate: ObjectInLibrary {}
        highlight: Rectangle {
            property var currentItem: objectsLibraryContainer.currentItem
            color: "yellow"
            width: parent ? parent.width : 0
            height: currentItem ? currentItem.height : 0
            y: currentItem ? currentItem.y : 0
        }
        highlightFollowsCurrentItem: false
        focus: true
        model: libraryTypeListModel
        footer: ObjectsLibraryListFooter {}
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}D{i:1}D{i:2}
}
##^##*/
