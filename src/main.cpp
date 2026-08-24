/**
 * @file main.cpp
 * @brief Тонкая точка входа архитектурной модели нисходящего канала NB-IoT.
 *
 * Программа строит только объектную топологию. Алгоритмы заполнения ресурсной
 * сетки намеренно не запускаются.
 */

#include <cstdlib>
#include <exception>
#include <iostream>

#include "model/DownlinkManager.h"
#include "model/Types.h"

namespace {

void printTopology(const nbiot::DownlinkManager& manager) {
    std::cout << "Топология NB-IoT успешно построена\n";
    for (const nbiot::Frame& frame : manager.frames()) {
        std::cout << "Кадр " << frame.frameNumber() << ':';
        for (const auto& subframe : frame.subframes()) {
            std::cout << " [" << subframe->context().subframeIndex << ": "
                      << nbiot::toString(subframe->type()) << ']';
        }
        std::cout << '\n';
    }
}

}  // namespace

int main() {
    try {
        nbiot::ModelConfig config;
        config.frameCount = 2;
        config.startFrameNumber = 0;
        config.cellId = 0;
        config.flexibleSubframes[1] = nbiot::SubframeType::Npdcch;
        config.flexibleSubframes[2] = nbiot::SubframeType::Npdsch;
        config.flexibleSubframes[9] = nbiot::SubframeType::Npdsch;

        nbiot::DownlinkManager manager(config);
        manager.buildTopology();
        printTopology(manager);

        std::cout << "Ресурсная сетка пока пуста: алгоритмы не реализованы.\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "Не удалось построить модель: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
