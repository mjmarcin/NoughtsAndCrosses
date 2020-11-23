import QtQuick 2.8


// Button class with customized text and state animation on presses.
Rectangle {
    id: rootRectangle
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 50
    anchors.horizontalCenter: parent.horizontalCenter
    width: 140
    height: 50
    border.width: 2

    property string buttonText
    signal buttonReleased()

    states: [
        State {
            name: "pressed"
            PropertyChanges {
                target: rootRectangle
                color: "black"
            }

            PropertyChanges {
                target: buttonLabel
                color: "white"
            }
        },

        State {
            name: "released"
            PropertyChanges {
                target: rootRectangle
                color: "white"
            }

            PropertyChanges {
                target: buttonLabel
                color: "black"
            }
        }
    ]

    Text {
        id: buttonLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        text: buttonText
        font.pixelSize: 24
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            rootRectangle.state = "pressed"
        }

        onReleased: {
            rootRectangle.state = "released"
            buttonReleased()
        }
    }
}
