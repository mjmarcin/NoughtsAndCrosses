import QtQuick 2.8
import QtQuick.Layouts 1.0

// Pane for displaying score information.
Rectangle {
    property int numberOfWins
    property int numberOfDraws

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Text {
                 anchors.centerIn: parent
                text: "Wins " + numberOfWins
                font.pointSize: 24
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Text {
                id: label
                anchors.centerIn: parent
                text: "Draws " + numberOfDraws
                font.pointSize: 24
            }
        }

    }
}
