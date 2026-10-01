#include "consolebackend.h"

#include "Correlator.h"
#include "NpssGenerator.h"
#include "NsssGenerator.h"

void ConsoleBackend::slotSetConsoleText() {
    NpssGenerator generator;
    Correlator correlator;
    NsssGenerator nsssGenerator {0, 0};
    auto npss_seq = generator.getNpssSequence();
    auto npss_correlation = correlator.cyclic_autocorrelation(npss_seq);
    auto nsss_seq = nsssGenerator.getNsssSequence();
    QString result {"NPSS: \n"};
    int c = 1;
    for (const auto &number : npss_seq) {
        QString valueString = "[" + QString::number(c) + "] " + QString::number(number.real()) + ":" + QString::number(number.imag()) + "\n";
        c++;

        result += valueString;
    }
    c = 1;
    result += "\nNPSS Correlation: \n";
    for (const auto &number : npss_correlation) {
        QString valueString = "[" + QString::number(c) + "] " + QString::number(number.real()) + ":" + QString::number(number.imag()) + "\n";
        c++;
        result += valueString;
    }
    c = 1;
    result += "\nNSSS: \n";
    for (const auto &number : nsss_seq) {
        QString valueString = "[" + QString::number(c) + "] " + QString::number(number.real()) + ":" + QString::number(number.imag()) + "\n";
        c++;
        result += valueString;
    }

    setConsoleText(result);
}
