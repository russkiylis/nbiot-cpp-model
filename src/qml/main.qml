import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    property int defaultWidth: 1280
    property int defaultHeight: 720

    minimumWidth: defaultWidth
    minimumHeight: defaultHeight
    visible: true
    title: "NB-IoT C++ Edition"

    SplitView {
        anchors.fill: parent
        anchors.margins: 10
        orientation: Qt.Vertical

        SplitView {
            orientation: Qt.Horizontal
            SplitView.minimumHeight: 450
            SplitView.fillHeight: true

            Rectangle {
                id: tabRectangle
                SplitView.fillWidth: true
                SplitView.minimumWidth: 600
                border.color: "gray"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 1
                    spacing: 0

                    TabBar {
                        id: mainTabBar
                        Layout.fillWidth: true

                        TabButton {
                            text: "Ресурсная сетка"
                        }

                        TabButton {
                            text: "Сигнал"
                        }

                    }

                    StackLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        currentIndex: mainTabBar.currentIndex

                        Loader {
                            active: StackLayout.isCurrentItem
                            source: "ResourceGridTab.qml"
                        }

                        Loader {
                            active: StackLayout.isCurrentItem
                            source: "SignalTab.qml"
                        }
                    }
                }
            }

            ConfigRectangle {}
        }

        Rectangle {
            id: consoleRectangle
            SplitView.fillHeight: true
            SplitView.minimumHeight: 80
            border.color: "gray"

            ScrollView {
                anchors.fill: parent
                clip: true

                TextArea{
                    placeholderText: "Здесь будет вывод значений..."
                    wrapMode: Text.Wrap
                    readOnly: true
                    text: app.consoleBackend.consoleText
                }
            }
        }
    }
}