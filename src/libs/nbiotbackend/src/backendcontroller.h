#pragma once
#include <QObject>

#include "configbackend.h"
#include "consolebackend.h"

// Контроллер всех подбекендов
class BackendController : public QObject
{
    Q_OBJECT

    // Засовываем подбекенды как Qпропертя
    Q_PROPERTY(ConsoleBackend* consoleBackend READ consoleBackend CONSTANT)
    Q_PROPERTY(ConfigBackend* configBackend READ configBackend CONSTANT)

    public:
        explicit BackendController(QObject *parent = nullptr) :
            QObject(parent),
            m_consoleBackend(std::make_unique<ConsoleBackend>()),
            m_configBackend(std::make_unique<ConfigBackend>())
        {
            connect(configBackend(), &ConfigBackend::signalEvalButtonPressed, consoleBackend(), &ConsoleBackend::slotSetConsoleText);
        };

        // Возвращаем сырой указатель
        ConsoleBackend* consoleBackend() const  { return m_consoleBackend.get(); }
        ConfigBackend* configBackend() const { return m_configBackend.get(); }


    private:
        std::unique_ptr<ConsoleBackend> m_consoleBackend;
        std::unique_ptr<ConfigBackend> m_configBackend;
};
