import QtQuick 2.8

// Class for creating crossing lines.
Rectangle {
    id: crossLine
    property alias opacityAnimatorRunning: opacityAnimator.running

    color: "black"
    opacity: 0

    NumberAnimation {
        id: opacityAnimator
        target: crossLine
        properties: "opacity"
        from: 0
        to: 1
        duration: 2000
    }
}
