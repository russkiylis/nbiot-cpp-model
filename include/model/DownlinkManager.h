/**
 * @file DownlinkManager.h
 * @brief Корневой менеджер архитектурной модели нисходящего канала NB-IoT.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "model/Frame.h"
#include "model/ModelConfig.h"
#include "model/NrsOverlay.h"
#include "model/ResourceGrid.h"

namespace nbiot {

/**
 * @brief Управляет конфигурацией, кадрами, сабфреймами и ресурсной сеткой.
 *
 * Жизненный цикл намеренно разделён на два шага. buildTopology() создаёт только
 * структуру объектов и пустую сетку. generateResourceGrid() показывает будущий
 * порядок алгоритмов, но до их реализации завершается std::logic_error.
 */
class DownlinkManager {
   public:
    /**
     * @brief Сохраняет и сразу проверяет конфигурацию модели.
     * @throws std::invalid_argument Если конфигурация некорректна.
     */
    explicit DownlinkManager(ModelConfig config);

    DownlinkManager(const DownlinkManager&) = delete;
    DownlinkManager& operator=(const DownlinkManager&) = delete;
    DownlinkManager(DownlinkManager&&) = delete;
    DownlinkManager& operator=(DownlinkManager&&) = delete;

    /**
     * @brief Создаёт кадры, сабфреймы и пустую ресурсную сетку.
     * @throws std::logic_error При повторном вызове.
     */
    void buildTopology();

    /**
     * @brief Последовательно запускает маппинг сабфреймов и наложение NRS.
     *
     * Заполнение выполняется во временной сетке. Менеджер публикует её только
     * после полного успеха, поэтому исключение не оставляет частичный результат.
     * @throws std::logic_error Если топология не построена или алгоритм текущего
     *         сабфрейма ещё не реализован.
     */
    void generateResourceGrid();

    [[nodiscard]] bool topologyBuilt() const noexcept;
    [[nodiscard]] const ModelConfig& config() const noexcept;

    /**
     * @brief Возвращает созданные кадры.
     * @throws std::logic_error Если buildTopology() ещё не вызывался.
     */
    [[nodiscard]] const std::vector<Frame>& frames() const;

    /**
     * @brief Возвращает пустую или в будущем заполненную ресурсную сетку.
     * @throws std::logic_error Если buildTopology() ещё не вызывался.
     */
    [[nodiscard]] const ResourceGrid& resourceGrid() const;

   private:
    [[nodiscard]] SubframeType resolveSubframeType(std::uint32_t frameNumber,
                                                   std::size_t subframeIndex) const;
    void requireBuiltTopology() const;

    ModelConfig config_;
    std::vector<Frame> frames_;
    std::optional<ResourceGrid> resourceGrid_;
    NrsOverlay nrsOverlay_;
    bool topologyBuilt_{false};
};

}  // namespace nbiot
