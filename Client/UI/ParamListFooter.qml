import QtQuick
import QtQuick.Controls

Item {
    Button {
        id: btnNewParam
        text: qsTr("Add new")
        onClicked: selectedObjectNewParamClicked()
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1}
}
##^##*/
