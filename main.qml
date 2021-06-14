import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Column {
        anchors.fill: parent
        spacing: 50

        Button {
            width: parent.width / 6
            height: width / 2
            text: "find devices"
            onClicked: {
                _deviceManager.startFindDevices()
            }
        }
    }
}
