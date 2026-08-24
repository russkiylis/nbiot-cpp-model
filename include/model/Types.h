/**
 * @file Types.h
 * @brief Общие типы архитектурного каркаса модели NB-IoT.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace nbiot {

/// Количество сабфреймов в одном радиокадре NB-IoT.
inline constexpr std::size_t kSubframesPerFrame = 10;

/// Количество поднесущих в одном сабфрейме NB-IoT.
inline constexpr std::size_t kSubcarriersPerSubframe = 12;

/// Количество OFDM-символов в одном сабфрейме.
inline constexpr std::size_t kSymbolsPerSubframe = 14;

/**
 * @brief Архитектурный тип сабфрейма.
 *
 * Значение описывает назначение всего сабфрейма. NRS сюда не входит, потому что
 * опорный сигнал накладывается поверх некоторых типов сабфреймов отдельной стадией.
 */
enum class SubframeType {
    Empty,
    Npbch,
    Npss,
    Nsss,
    Npdsch,
    Npdcch,
};

/**
 * @brief Источник значения отдельного ресурсного элемента.
 *
 * В отличие от SubframeType этот enum содержит NRS: один сабфрейм может нести
 * основной канал и дополнительно опорный сигнал.
 */
enum class ResourceElementType {
    Empty,
    Npbch,
    Npss,
    Nsss,
    Npdsch,
    Npdcch,
    Nrs,
};

/**
 * @brief Не владеющий данными контекст конкретного сабфрейма.
 *
 * Контекст передаётся сабфрейму при создании и остаётся неизменным на протяжении
 * всей его жизни. Это позволяет не хранить обратные указатели на Frame и
 * DownlinkManager.
 */
struct SubframeContext {
    std::size_t frameIndex{};     ///< Индекс кадра внутри текущей модели.
    std::uint32_t frameNumber{};  ///< Абсолютный номер радиокадра.
    std::size_t subframeIndex{};  ///< Номер сабфрейма: от 0 до 9.
    std::uint16_t cellId{};       ///< Идентификатор соты.
};

/// Возвращает короткое человекочитаемое имя типа сабфрейма.
[[nodiscard]] std::string_view toString(SubframeType type) noexcept;

/// Сообщает, разрешено ли накладывать NRS на данный тип сабфрейма.
[[nodiscard]] bool supportsNrs(SubframeType type) noexcept;

}  // namespace nbiot
