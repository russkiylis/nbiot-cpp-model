import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphs

ApplicationWindow {
    id: root
    visible: true
    width: 1400
    height: 1050
    title: "FFT graphs"

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            x: 16
            y: 16
            width: root.width - 32
            spacing: 18

            Label {
                text: "Signal reconstruction"
                font.pixelSize: 18
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 240

                GraphsView {
                    anchors.fill: parent
                    axisX: ValueAxis {
                        min: 0
                        max: plotProvider.sampleCount - 1
                    }
                    axisY: ValueAxis {
                        min: plotProvider.signalMin - 0.2
                        max: plotProvider.signalMax + 0.2
                    }

                    LineSeries {
                        id: inputSeries
                        name: "Input"
                        color: "#2563eb"
                    }

                    LineSeries {
                        id: reconstructedSeries
                        name: "IFFT(FFT(x))"
                        color: "#f97316"
                    }
                }
            }

            Label {
                text: "Spectrum comparison"
                font.pixelSize: 18
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 240

                GraphsView {
                    anchors.fill: parent
                    axisX: ValueAxis {
                        min: 0
                        max: plotProvider.sampleCount - 1
                    }
                    axisY: ValueAxis {
                        min: 0
                        max: plotProvider.spectrumMax * 1.1
                    }

                    LineSeries {
                        id: dftSeries
                        name: "DFT"
                        color: "#16a34a"
                    }

                    LineSeries {
                        id: fftSeries
                        name: "FFT"
                        color: "#7c3aed"
                    }
                }
            }

            Label {
                text: "Bit channels"
                font.pixelSize: 18
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 240

                GraphsView {
                    anchors.fill: parent
                    axisX: ValueAxis {
                        min: 0
                        max: plotProvider.sampleCount - 1
                    }
                    axisY: ValueAxis {
                        min: plotProvider.bitsMin - 0.1
                        max: plotProvider.bitsMax + 0.1
                    }

                    LineSeries { id: bits0; color: "#dc2626" }
                    LineSeries { id: bits1; color: "#2563eb" }
                    LineSeries { id: bits2; color: "#16a34a" }
                    LineSeries { id: bits3; color: "#7c3aed" }
                }
            }

            Label {
                text: "BPSK channels"
                font.pixelSize: 18
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 240

                GraphsView {
                    anchors.fill: parent
                    axisX: ValueAxis {
                        min: 0
                        max: plotProvider.sampleCount - 1
                    }
                    axisY: ValueAxis {
                        min: plotProvider.bpskMin - 0.1
                        max: plotProvider.bpskMax + 0.1
                    }

                    LineSeries { id: bpsk0; color: "#dc2626" }
                    LineSeries { id: bpsk1; color: "#2563eb" }
                    LineSeries { id: bpsk2; color: "#16a34a" }
                    LineSeries { id: bpsk3; color: "#7c3aed" }
                }
            }
        }
    }

    function fillSeries(series, points) {
        series.clear()
        for (let i = 0; i < points.length; ++i) {
            series.append(points[i].x, points[i].y)
        }
    }

    Component.onCompleted: {
        fillSeries(inputSeries, plotProvider.inputSignalPoints())
        fillSeries(reconstructedSeries, plotProvider.reconstructedSignalPoints())
        fillSeries(dftSeries, plotProvider.dftSpectrumPoints())
        fillSeries(fftSeries, plotProvider.fftSpectrumPoints())
        fillSeries(bits0, plotProvider.bitsPoints(0))
        fillSeries(bits1, plotProvider.bitsPoints(1))
        fillSeries(bits2, plotProvider.bitsPoints(2))
        fillSeries(bits3, plotProvider.bitsPoints(3))
        fillSeries(bpsk0, plotProvider.bpskPoints(0))
        fillSeries(bpsk1, plotProvider.bpskPoints(1))
        fillSeries(bpsk2, plotProvider.bpskPoints(2))
        fillSeries(bpsk3, plotProvider.bpskPoints(3))
    }
}
