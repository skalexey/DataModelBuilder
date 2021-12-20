import QtQuick 2.0
import ObjectModel 1.0
import DataModelBuilderUI 1.0

Item {
    width: Constants.width
    height: Constants.height
    DataModelBuilderUI {
        objectsLibrary.gridView.model: ObjectListModel {}
    }
}
