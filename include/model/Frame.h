/**
 * @file Frame.h
 * @brief Контейнер одного радиокадра и десяти его сабфреймов.
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "model/Subframe.h"
#include "model/Types.h"

namespace nbiot {

/**
 * @brief Радиокадр, который единолично владеет десятью сабфреймами.
 */
class Frame {
   public:
    using Subframes = std::array<std::unique_ptr<Subframe>, kSubframesPerFrame>;

    /**
     * @brief Принимает владение десятью согласованными сабфреймами кадра.
     * @param frameNumber Абсолютный номер радиокадра.
     * @param subframes Массив единственных владеющих указателей.
     * @throws std::invalid_argument Если указатель пуст или контекст сабфрейма
     *         не соответствует кадру и позиции в массиве.
     */
    Frame(std::uint32_t frameNumber, Subframes subframes);

    Frame(const Frame&) = delete;
    Frame& operator=(const Frame&) = delete;
    Frame(Frame&&) noexcept = default;
    Frame& operator=(Frame&&) noexcept = default;

    /// Возвращает абсолютный номер радиокадра.
    [[nodiscard]] std::uint32_t frameNumber() const noexcept;

    /// Возвращает массив владеющих указателей только для чтения.
    [[nodiscard]] const Subframes& subframes() const noexcept;

    /**
     * @brief Возвращает сабфрейм по номеру от 0 до 9.
     * @throws std::out_of_range При неверном номере.
     */
    [[nodiscard]] const Subframe& subframe(std::size_t subframeIndex) const;

   private:
    std::uint32_t frameNumber_{};
    Subframes subframes_;
};

}  // namespace nbiot
