import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Palm1r.networkProject.RootController 1.0

Window {
   id: root

    width: 640
    height: 480
    visible: true
    title: qsTr("Network Test Project")

    Row {
        anchors.centerIn: parent
        spacing: 20

        HubButton {
            id: serverButton

            buttonColor: "lightblue"
            buttonText: "Start server"
            onButtonClicked: {
                opacity = 0.0
                deviceList.opacity = 1.0
                RootController.findDevices()
            }
            Behavior on opacity {NumberAnimation{duration: 250}}
        }

        HubButton {
            buttonColor: "lightgreen"
            buttonText: "Start client"

            TextField {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                }

                horizontalAlignment: TextInput.AlignHCenter
                placeholderText: qsTr("Enter id")
                text: "UniqueID"
                onTextChanged: {
                    RootController.setDeviceId(text)
                }
            }
        }
    }

    ListView {
        id: deviceList

        anchors{
            fill: parent
            margins: 50
        }
        z: -1
        opacity: 0.0
        model: RootController.deviceModel //5
        spacing: 20
        delegate: Rectangle {
            width: childrenRect.width
            height: childrenRect.height
            color: "lightblue"
            radius: width / 10
            border.width: 1
            border.color: Qt.darker(color)

            Column {
                padding: 20
                spacing: 20

                Text {
                    text: deviceId
                }
                Text {
                    text: deviceIp
                }
                Text {
                    text: deviceMessageCount
                }
            }
        }
    }
}
