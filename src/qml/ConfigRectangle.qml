import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: configRectangle
    SplitView.fillWidth: true
    SplitView.minimumWidth: 400
    border.color: "gray"

    Button {
        anchors.centerIn: parent
        id: evalButton
        text: "Вычислить"

        onClicked: {
            app.configBackend.evalButtonPressed()
        }
    }
}