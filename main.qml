import QtQuick 2.15
import QtQuick.Window 2.15
import ObjectModel 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Data Model Builder")

    DesignForm {
        gview.model: ObjectListModel {
        }
    }
}
