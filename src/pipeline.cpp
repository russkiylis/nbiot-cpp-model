#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#ifdef _WIN32
    #include <windows.h>
#endif

#include <iostream>
#include <iomanip>

#include "libs/nbiotbackend/src/backendcontroller.h"
#include "libs/nbiotarchitecture/src/FrameGen.h" 

void runNbiotFrameTest() {
    std::cout << "\n=== Запуск теста генерации NB-IoT Frame ===\n";
    
    nbiot::FrameGenerator frameGen;
    
    // Генерируем 2 фрейма для теста
    size_t numFrames = 1;
    // Примечание: если ваш метод называется generate(), замените generateFrames на generate
    auto frames = frameGen.generateFrames(numFrames); 

    std::cout << "Generated " << numFrames << " NB-IoT Frame(s).\n";
    std::cout << "Subframe size: 12 subcarriers x 14 symbols\n\n";

    // Проверяем 5-й сабфрейм (индекс 5) в первом фрейме (именно там живет NPSS)
    size_t targetFrame = 0;
    size_t targetSubframe = 5;
    
    std::cout << "--- Frame " << targetFrame << ", Subframe " << targetSubframe << " (NPSS) ---\n";
    std::cout << "Format: grid[subcarrier][symbol] (non-zero values only)\n\n";
    
    // Проходим по сетке и выводим только заполненные NPSS элементы
    for (size_t k = 0; k < 12; ++k) {
        for (size_t l = 0; l < 14; ++l) {
            // Обратите внимание: синтаксис доступа зависит от вашей реализации ResourceGrid.
            // Если это std::array<std::array<complex, 14>, 12>, то доступ такой:
            auto val = frames[targetFrame][targetSubframe][k][l];
            
            // Выводим только ненулевые элементы, чтобы не захламлять консоль
            if (val.real() != 0.0f || val.imag() != 0.0f) {
                std::cout << "[" << k << "," << std::setw(2) << l << "]: " 
                          << std::fixed << std::setprecision(2) 
                          << val.real() << (val.imag() >= 0 ? "+" : "") << val.imag() << "j  ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "=== Тест завершен ===\n\n";
}

int main(int argc, char *argv[])
{

#ifdef _WIN32
    // Включаем поддержку UTF-8 в консоли (только для Windows)
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::setlocale(LC_ALL, ".UTF-8");

    // Меняем режим консоли на UTF-8
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
    
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    runNbiotFrameTest();

    QQmlApplicationEngine engine;

    BackendController backendController;
    engine.rootContext()->setContextProperty("app", &backendController);

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

