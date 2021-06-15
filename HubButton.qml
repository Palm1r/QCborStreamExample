import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Palm1r.networkProject.RootController 1.0

Rectangle {
    id: root

    property alias buttonText: buttonText.text
    property color buttonColor
    signal buttonClicked

    width: 200
    height: 100
    radius: width / 10
    color: activeArea.containsMouse ? Qt.lighter(buttonColor)
                                    : buttonColor
    border.width: 1
    border.color: Qt.darker(color)

    Text {
        id: buttonText

        anchors.centerIn: parent
    }

    MouseArea {
        id: activeArea

        anchors.fill: parent
        hoverEnabled: true
        onPressed: root.color = Qt.darker(buttonColor)
        onReleased: root.color = buttonColor
        onClicked: root.buttonClicked()
    }
}
