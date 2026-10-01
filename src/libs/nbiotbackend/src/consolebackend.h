#pragma once

#include <QObject>

class ConsoleBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString consoleText READ consoleText WRITE setConsoleText NOTIFY consoleTextChanged)

    public:
        explicit ConsoleBackend(QObject* parent = nullptr): QObject(parent) {};

        QString consoleText() const { return m_consoleText; }
        void setConsoleText(const QString& consoleText)
        {
            m_consoleText = consoleText;
            emit consoleTextChanged();
        };

    public slots:
        void slotSetConsoleText();

    signals:
        void consoleTextChanged();

    private:
        QString m_consoleText;
};
