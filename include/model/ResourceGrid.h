/**
 * @file ResourceGrid.h
 * @brief Типизированное хранилище ресурсной сетки без алгоритмов заполнения.
 */

#pragma once

#include <complex>
#include <cstddef>
#include <vector>

#include "model/Types.h"

namespace nbiot {

/**
 * @brief Один элемент ресурсной сетки.
 *
 * В заготовке элементы создаются нулевыми. Поле type в будущем позволит отличать
 * каналы и сигналы без отдельного числового слоя с цветовыми индексами.
 */
struct ResourceElement {
    std::complex<float> value{};
    ResourceElementType type{ResourceElementType::Empty};
};

/**
 * @brief Ресурсная сетка для всех кадров, которыми управляет DownlinkManager.
 *
 * Класс реализует только хранение и безопасную адресацию. Формулы генерации и
 * маппинга физических каналов в нём отсутствуют.
 */
class ResourceGrid {
   public:
    /**
     * @brief Создаёт пустую сетку для указанного числа кадров.
     * @throws std::invalid_argument Если frameCount равен нулю.
     * @throws std::length_error Если число элементов превышает max_size хранилища.
     */
    explicit ResourceGrid(std::size_t frameCount);

    /// Возвращает число кадров, для которых выделена сетка.
    [[nodiscard]] std::size_t frameCount() const noexcept;

    /// Возвращает полное число ресурсных элементов во всех кадрах.
    [[nodiscard]] std::size_t elementCount() const noexcept;

    /**
     * @brief Возвращает изменяемый ресурсный элемент по четырём индексам.
     * @throws std::out_of_range Если хотя бы один индекс находится вне сетки.
     */
    ResourceElement& at(std::size_t frameIndex, std::size_t subframeIndex,
                        std::size_t subcarrierIndex, std::size_t symbolIndex);

    /**
     * @brief Возвращает ресурсный элемент по четырём индексам.
     * @throws std::out_of_range Если хотя бы один индекс находится вне сетки.
     */
    const ResourceElement& at(std::size_t frameIndex, std::size_t subframeIndex,
                              std::size_t subcarrierIndex, std::size_t symbolIndex) const;

   private:
    [[nodiscard]] std::size_t flatIndex(std::size_t frameIndex, std::size_t subframeIndex,
                                        std::size_t subcarrierIndex, std::size_t symbolIndex) const;

    std::size_t frameCount_{};
    std::vector<ResourceElement> elements_;
};

}  // namespace nbiot
