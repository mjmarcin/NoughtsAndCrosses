import QtQuick 2.8

// Class representing tile on the board.
Rectangle {
    property alias fronSideZ: frontSide.z
    property alias imageSource: image.source

    Image {
        id: image
        anchors.fill: parent
    }

    Rectangle {
        id: frontSide
        anchors.fill: parent
        color: "white"
    }
}
