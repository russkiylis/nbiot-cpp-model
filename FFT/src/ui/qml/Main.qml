import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphs

ApplicationWindow {
    id: root
    visible: true
    width: 1400
    height: 1200
    title: "OFDM instant spectra"

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            x: 16
            y: 16
            width: root.width - 32
            spacing: 18

            Label {
                text: "Inst spec 1"
                font.pixelSize: 18
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 240

                GraphsView {
                    anchors.fill: parent
                    axisX: BarCategoryAxis { id: categories0 }
                    axisY: ValueAxis {
                        min: 0
                        max: plotProvider.spectrumMax * 1.1
                    }

                    BarSeries {
                        id: spectrum0
                        barWidth: 0.3
                        BarSet { id: spectrum0set; color: "#FF0000" }
                    }
                }
            }

            Label {
                text: "Inst speс 2" 
                font.pixelSize: 18
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 240

                GraphsView {
                    anchors.fill: parent
                    axisX: BarCategoryAxis { id: categories1 }
                    axisY: ValueAxis {
                        min: 0
                        max: plotProvider.spectrumMax * 1.1
                    }

                    BarSeries {
                        id: spectrum1
                        barWidth: 0.3
                        BarSet { id: spectrum1set; color: "#FF0000" }
                    }
                }
            }

            Label {
                text: "Inst spec 3"
                font.pixelSize: 18
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 240

                GraphsView {
                    anchors.fill: parent
                    axisX: BarCategoryAxis { id: categories2 }
                    axisY: ValueAxis {
                        min: 0
                        max: plotProvider.spectrumMax * 1.1
                    }

                    BarSeries {
                        id: spectrum2
                        barWidth: 0.3
                        BarSet { id: spectrum2set; color: "#FF0000" }
                    }
                }
            }

            Label {
                text: "Avg spec"
                font.pixelSize: 18
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 240

                GraphsView {
                    anchors.fill: parent
                    axisX: BarCategoryAxis { id: categoriesAvg }
                    axisY: ValueAxis {
                        min: 0
                        max: plotProvider.spectrumMax * 1.1
                    }

                    BarSeries {
                        id: averageSpectrum
                        barWidth: 0.3
                        BarSet { id: averageSpectrumSet; color: "#FF0000" }
                    }
                }
            }
        }
    }

    function makeCategories(count) {
        const categories = []
        for (let i = 0; i < count; ++i) {
            categories.push(String(i))
        }
        return categories
    }

    function fillBarSet(barSet, points) {
        const values = []
        for (let i = 0; i < points.length; ++i) {
            values.push(points[i].y)
        }
        barSet.values = values
    }

    Component.onCompleted: {
        const categories = makeCategories(plotProvider.windowSize)
        categories0.categories = categories
        categories1.categories = categories
        categories2.categories = categories
        categoriesAvg.categories = categories

        fillBarSet(spectrum0set, plotProvider.spectrumPoints(0))
        fillBarSet(spectrum1set, plotProvider.spectrumPoints(1))
        fillBarSet(spectrum2set, plotProvider.spectrumPoints(2))
        fillBarSet(averageSpectrumSet, plotProvider.averageSpectrumPoints())
    }
}
