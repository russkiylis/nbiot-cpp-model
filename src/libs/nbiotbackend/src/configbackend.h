#pragma once

#include <QObject>

class ConfigBackend : public QObject
{
    Q_OBJECT

    public:
        explicit ConfigBackend(QObject *parent = nullptr) : QObject(parent) {};

        Q_INVOKABLE void evalButtonPressed()
        {
            qDebug("evalButtonPressed");
            emit signalEvalButtonPressed();
        }
    signals:
        void signalEvalButtonPressed();
};
