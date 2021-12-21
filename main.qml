import QtQuick 2.0
import ObjectModel 1.0
import DataModelBuilderUI 1.0

Item {
    width: Constants.width
    height: Constants.height
    property ObjectListModel m: ObjectListModel {};
    DataModelBuilderUI {
        objectsLibrary.gridView.model: m
        selectedObject.params.model: m.objectParamListModel(objectsLibrary.gridView.currentIndex)
        selectedObject.objectName.text: objectsLibrary.gridView.currentItem.objectName.text
    }
}
