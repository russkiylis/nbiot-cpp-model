/**
 * @file Subframe.h
 * @brief Абстрактный контракт любого сабфрейма архитектурной модели.
 */

#pragma once

#include "model/ResourceGrid.h"
#include "model/Types.h"

namespace nbiot {

/**
 * @brief Базовый абстрактный класс сабфрейма.
 *
 * Класс хранит только общий контекст и задаёт точку расширения mapTo(). Конкретный
 * физический канал выбирается полиморфно через производный класс.
 */
class Subframe {
   public:
    /**
     * @brief Сохраняет неизменяемый контекст будущего сабфрейма.
     * @throws std::invalid_argument Если номер сабфрейма не входит в диапазон 0–9.
     */
    explicit Subframe(SubframeContext context);
    virtual ~Subframe() = default;

    Subframe(const Subframe&) = delete;
    Subframe& operator=(const Subframe&) = delete;
    Subframe(Subframe&&) = delete;
    Subframe& operator=(Subframe&&) = delete;

    /// Возвращает сохранённый не владеющий контекст сабфрейма.
    [[nodiscard]] const SubframeContext& context() const noexcept;

    /// Возвращает физическое назначение конкретного сабфрейма.
    [[nodiscard]] virtual SubframeType type() const noexcept = 0;

    /// Сообщает, должна ли будущая стадия накладывать NRS.
    [[nodiscard]] bool allowsNrs() const noexcept;

    /**
     * @brief Размещает канал или сигнал сабфрейма в общей ресурсной сетке.
     *
     * В архитектурной заготовке конкретные реализации намеренно выбрасывают
     * std::logic_error: численные алгоритмы ещё не добавлены.
     */
    virtual void mapTo(ResourceGrid& resourceGrid) const = 0;

   protected:
    /// Формирует единообразную ошибку для пока не реализованных алгоритмов.
    [[noreturn]] void throwMappingNotImplemented() const;

   private:
    SubframeContext context_;
};

}  // namespace nbiot
