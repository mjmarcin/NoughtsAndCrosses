import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import enums 1.0


// Main application window.
Window {
    id: mainWindow
    visible: true
    maximumWidth: 800
    maximumHeight: 480
    minimumWidth: 800
    minimumHeight: 480
    title: qsTr("Naughts and Crosses")

    readonly property int crossLineHeight: 8
    readonly property string crossImage: "qrc:/Images/cross.png"
    readonly property string noughtImage: "qrc:/Images/nought.png"

    // Main pane containg boar pane and information pane.
    Rectangle {
        id: mainPane
        anchors.fill: parent
        property int boardSize: controller.boardSize()

        Rectangle {
            id: boardPane
            height: mainPane.height
            width: mainPane.height
            color: "black"

            // Grid containing the board with tiles.
            GridLayout {
                id: gridLayout
                anchors.fill: boardPane
                rows: mainPane.boardSize
                columns: mainPane.boardSize
                property int spacing: rowSpacing

                // Total tile height including space between tiles. Calculated based on board size and grid dimensions.
                property int totalTileHeight: (boardPane.height - (mainPane.boardSize - 1) * gridLayout.spacing ) / mainPane.boardSize + gridLayout.spacing

                // Half height of the tile itself. Calculated based on board size and grid dimensions.
                property int halfTileHeight: boardPane.height / (mainPane.boardSize * 2) - gridLayout.spacing

                Repeater {
                    id: repeater
                    model: gridLayout.rows * gridLayout.columns


                    Tile {
                        id: tileRectangle
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        property alias angleAnimation: onAngle
                        property alias angleValue: rotation.angle

                        transform: Rotation {
                            id: rotation;
                            origin.x: width / 2;
                            origin.y: height / 2;
                            axis { x: 0; y: 1; z: 0 }
                            angle: 0

                            Behavior on angle {
                                id: onAngle

                                NumberAnimation {
                                    id: animation
                                    duration: 500
                                }
                            }

                            onAngleChanged: {
                                if (tileRectangle.fronSideZ !== -1 && angle > 90) {
                                    tileRectangle.fronSideZ = -1
                                }

                                if (tileRectangle.fronSideZ === -1 && angle < 90) {
                                    tileRectangle.fronSideZ = 0
                                }
                            }
                        }

                        Connections {
                            target: controller
                            onTileStateChanged: {

                            // Update corresponding tile with the current player's symbol.
                            if (index == tileIndex) {
                                    var tileType = controller.getTileType(index)

                                    if (tileType === Enums.Cross) {
                                        tileRectangle.imageSource = crossImage
                                        rotation.angle = 180
                                    }
                                    else if (tileType ===  Enums.Nought) {
                                        tileRectangle.imageSource = noughtImage
                                        rotation.angle = 180
                                    }
                                }
                            }
                        }

                        MouseArea {
                            enabled: controller.roundStatus === Enums.NotFinished
                            anchors.fill: parent;
                            onClicked: {
                                if (controller.getTileType(index) === Enums.Empty) {
                                    controller.updateTileState(index)
                                }
                            }
                        }
                    }
                }
            }

            CrossLine {
                id: horizontalCrossLine
                anchors.left: parent.left
                anchors.right: parent.right
                height: mainWindow.crossLineHeight

                property int rowIndex
                y: rowIndex * gridLayout.totalTileHeight + gridLayout.halfTileHeight
            }

            CrossLine {
                id: verticalCenterCrossLine

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: mainWindow.crossLineHeight

                property int columnIndex
                x: columnIndex * gridLayout.totalTileHeight + gridLayout.halfTileHeight
            }

            CrossLine {
                id: downDiagonalCorssLine
                color: "black"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: - 0.1 * boardPane.width
                anchors.rightMargin: - 0.1 * boardPane.width
                height: mainWindow.crossLineHeight
                y: gridLayout.totalTileHeight + gridLayout.halfTileHeight

                transform: Rotation {
                    origin.x: downDiagonalCorssLine.width / 2
                    origin.y: downDiagonalCorssLine.height / 2
                    angle: 45
                }
            }

            CrossLine {
                id: upDiagonalCorssLine
                color: "black"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: - 0.1 * boardPane.width
                anchors.rightMargin: - 0.1 * boardPane.width
                height: mainWindow.crossLineHeight
                y: gridLayout.totalTileHeight + gridLayout.halfTileHeight

                transform: Rotation {
                    origin.x: downDiagonalCorssLine.width / 2
                    origin.y: downDiagonalCorssLine.height / 2
                    angle: 135
                }
            }
        }

        // Information pane with round results.
        Rectangle {
            id: roundResultPane
            anchors.left: boardPane.right
            anchors.right: mainPane.right
            height: parent.height

            Column {
                anchors.centerIn: parent

                Text {
                    id: roundResultLabel
                    text: controller.roundStatus === Enums.FinishedDraw ? "Draw" :
                          controller.roundStatus === Enums.FinishedWin ? "Winner" : ""
                    font.pixelSize: 30
                }

                Image {
                    id: img
                    fillMode: Image.PreserveAspectFit
                    source: controller.currentPlayer === Enums.PlayerX ? "qrc:/Images/cross.png" : "qrc:/Images/nought.png"
                    visible: controller.roundStatus === Enums.FinishedWin
                 }
            }

            Button {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                buttonText: "Next round"

                onButtonReleased: {
                    inAnimation.running = true
                }
            }
        }

        // Player infomation pane with current player and the score.
        Rectangle {
            id: playerInformation
            anchors.left: boardPane.right
            anchors.right: mainPane.right
            height: parent.height

            NumberAnimation {
                id: outAnimation
                target: playerInformation
                properties: "y"
                from: 0;
                to: playerInformation.height
                duration: 500
                running: controller.roundStatus !== Enums.NotFinished
            }

            NumberAnimation {
                id: inAnimation
                target: playerInformation
                properties: "y"
                from: playerInformation.height
                to: 0
                duration: 500

                onRunningChanged: {
                    if (running == false) {
                        controller.startNextRound()

                        for (var i = 0; i < mainPane.boardSize * mainPane.boardSize; i++) {
                            var tile = repeater.itemAt(i)
                            tile.angleValue = 0
                        }

                        horizontalCrossLine.opacity = 0
                        horizontalCrossLine.opacityAnimatorRunning = false
                        verticalCenterCrossLine.opacity = 0
                        verticalCenterCrossLine.opacityAnimatorRunning = false
                        upDiagonalCorssLine.opacity = 0
                        upDiagonalCorssLine.opacityAnimatorRunning = false
                        downDiagonalCorssLine.opacity = 0
                        downDiagonalCorssLine.opacityAnimatorRunning = false
                    }
                }
            }

            CurrentPlayerPane {
                id: currentPlayerPane
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 0.4 * parent.height
            }

            ScorePane {
                id: scorePlayerPane
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: currentPlayerPane.bottom
                height: 0.4 * parent.height
                numberOfWins: controller.winsNumber
                numberOfDraws: controller.drawsNumber
            }
        }
    }

    Connections {
        target: controller
        onLineCompleted: {
            // Make crossing lines visible depending for completed lines.
            if (lineType === Enums.HorizontalLine)
            {
                horizontalCrossLine.rowIndex = index
                horizontalCrossLine.opacityAnimatorRunning = true
            }
            else if (lineType === Enums.VerticalLine)
            {
                verticalCenterCrossLine.columnIndex = index
                verticalCenterCrossLine.opacityAnimatorRunning = true
            }
            else if (lineType === Enums.DownDiagonalLine)
            {
                downDiagonalCorssLine.opacityAnimatorRunning = true
            }
            else if (lineType === Enums.UpDiagonalLine)
            {
                upDiagonalCorssLine.opacityAnimatorRunning = true
            }
        }
    }
}
