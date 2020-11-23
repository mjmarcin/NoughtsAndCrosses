import QtQuick 2.8
import enums 1.0

// Pane with current player image with text label.
Rectangle {
    color: "transparent"

    Rectangle {
        id: labelPane
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height * 0.4

        Text {
            id: currentPlayerLabel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: "Current player"
            font.pixelSize: 16
        }
    }

    Rectangle {
        id: playerImagePane
        anchors.top: labelPane.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Image {
            id: currentPlayerImage
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: controller.currentPlayer === Enums.PlayerX ? mainWindow.crossImage : mainWindow.noughtImage

            onSourceChanged: {
                opacityAnimation.running = true
            }

            NumberAnimation {
                id: opacityAnimation
                target: currentPlayerImage
                properties: "opacity"
                from: 0
                to: 1
                duration: 500
            }
        }
    }
}
