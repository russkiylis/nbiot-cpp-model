#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStyleHints>
#include <QString>
#include "GraphDataProvider.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.styleHints()->setColorScheme(Qt::ColorScheme::Light);

    GraphDataProvider provider;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("plotProvider"), &provider);
    engine.loadFromModule("FFTApp", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
