import QtQuick
import QtQuick.Controls

Item {
	id: item1
	width: 300
	height: 1080

	property alias libraryContainer: objectsLibraryContainer

	Text {
		id: objectsLibraryTitle
		x: 0
		y: 0
		width: parent.width
		height: 40
		text: qsTr("Type Library")
		font.pixelSize: 24
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
	}

	InteractiveListView {
		id: objectsLibraryContainer
		width: parent.width * 0.8
		y: objectsLibraryTitle.y + objectsLibraryTitle.height
		height: parent.height - objectsLibraryTitle.y - objectsLibraryTitle.height
		anchors.horizontalCenter: parent.horizontalCenter
		delegate: ObjectInLibrary {
			width: objectsLibraryContainer.width // To fix the error "TypeError: Cannot read property 'width' of null"
			container: objectsLibraryContainer
			getName: function() { return name; }
			setName: function(val) { name = val; }
			getValue: function() { return value; }
			setValue: function(val) { value = val; }
			getValueStr: function() { return valueStr; }
			getType: function() { return type; }
			setType: function(val) { type = val; }
			getTypeStr: function() { return typeStr; }
			getParentModel: function() { return parentModel; }

		}
		model: libraryTypeListModel
		footer: ObjectsLibraryListFooter {}
	}
}

/*##^##
Designer {
	D{i:0;formeditorZoom:0.66}D{i:1}D{i:2}
}
##^##*/

