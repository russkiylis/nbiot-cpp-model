# Разбор выбранных задач

Этот файл не является списком ответов для механического копирования. Открывай
нужный раздел только после собственной попытки и хотя бы одной проверки.

Полезный способ работы с разбором:

1. Сравни сначала контракты, а не строки кода.
2. Найди одно отличие от своей версии.
3. Объясни, является ли оно исправлением, альтернативой или вкусовым выбором.
4. Закрой файл.
5. На следующий день воспроизведи решение без подсматривания.

У многих задач есть несколько корректных решений. Здесь показан один ясный
вариант, соответствующий стилю учебника.

## После главы 3: `sortTwo` и `reciprocal`

Контракты:

- `sortTwo` изменяет оба аргумента так, чтобы первый не превосходил второй;
- `reciprocal` возвращает `false` для нулевого входа и в этом случае не меняет
  выходной параметр;
- для ненулевого входа `reciprocal` записывает обратное значение и возвращает
  `true`.

```cpp
#include <iostream>

void sortTwo(double& first, double& second)
{
    if (first > second) {
        const double temporary{first};
        first = second;
        second = temporary;
    }
}

bool reciprocal(double input, double& result)
{
    if (input == 0.0) {
        return false;
    }

    result = 1.0 / input;
    return true;
}

int main()
{
    double low{9.0};
    double high{2.0};
    sortTwo(low, high);
    std::cout << low << ' ' << high << '\n';

    double result{123.0};
    if (reciprocal(4.0, result)) {
        std::cout << result << '\n';
    }

    if (!reciprocal(0.0, result)) {
        std::cout << "no reciprocal; old result = " << result << '\n';
    }
}
```

Почему `input` передаётся по значению? Это одно маленькое число, функция его
не должна менять снаружи. Почему `result` — ссылка? Изменение вызывающего
объекта является частью выбранного контракта.

Позже, после главы об ошибках, можно обсудить более выразительный вариант с
`std::optional<double>`, который не требует выходного параметра.

## После главы 4: фильтрация и `join`

```cpp
#include <iostream>
#include <string>
#include <vector>

std::vector<int> evenValues(const std::vector<int>& values)
{
    std::vector<int> result;
    result.reserve(values.size());

    for (const int value : values) {
        if (value % 2 == 0) {
            result.push_back(value);
        }
    }

    return result;
}

std::string join(
    const std::vector<std::string>& parts,
    const std::string& separator)
{
    std::string result;

    for (std::size_t i{0}; i < parts.size(); ++i) {
        if (i != 0) {
            result += separator;
        }
        result += parts[i];
    }

    return result;
}

int main()
{
    const std::vector<int> values{5, 2, 8, 3, 10};
    for (const int value : evenValues(values)) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    const std::vector<std::string> names{"alpha", "beta", "gamma"};
    std::cout << join(names, " | ") << '\n';
    std::cout << '[' << join({}, ", ") << "]\n";
}
```

`reserve(values.size())` не создаёт элементы. Он только заранее выделяет
достаточную ёмкость. Результат фильтра может оказаться короче исходного, но
никогда не будет длиннее, поэтому такая оценка безопасна.

В `join` разделитель добавляется **перед** каждым элементом, кроме первого.
Это автоматически обрабатывает пустой список и не оставляет лишний
разделитель в конце.

## После главы 6: медиана без изменения исходного вектора

Контракт выбран такой:

- пустой набор не имеет медианы;
- для нечётного размера возвращается центральный элемент;
- для чётного — среднее двух центральных;
- исходный контейнер не меняется.

```cpp
#include <algorithm>
#include <iostream>
#include <optional>
#include <vector>

std::optional<double> median(const std::vector<double>& values)
{
    if (values.empty()) {
        return std::nullopt;
    }

    std::vector<double> sorted{values};
    std::sort(sorted.begin(), sorted.end());

    const std::size_t middle{sorted.size() / 2};
    if (sorted.size() % 2 == 1) {
        return sorted[middle];
    }

    return (sorted[middle - 1] + sorted[middle]) / 2.0;
}

int main()
{
    const std::vector<double> odd{7.0, 1.0, 4.0};
    const std::vector<double> even{8.0, 2.0, 4.0, 6.0};

    std::cout << *median(odd) << '\n';
    std::cout << *median(even) << '\n';
    std::cout << std::boolalpha << median({}).has_value() << '\n';
}
```

Копия здесь осмысленна: `std::sort` меняет контейнер, а контракт запрещает
изменять исходный вектор. Для огромных наборов можно искать медиану более
специализированным алгоритмом, но сначала нужен корректный ясный вариант.

В рабочем коде перед разыменованием `optional` нужно проверять наличие
значения. В примере два первых входа заведомо непусты, но это локальное знание
не следует бездумно переносить в общий код.

## После главы 7: `ClosedInterval` с инвариантом

```cpp
#include <cmath>
#include <iostream>
#include <stdexcept>

class ClosedInterval {
public:
    ClosedInterval(double minimum, double maximum)
        : minimum_{minimum}
        , maximum_{maximum}
    {
        if (!std::isfinite(minimum_) || !std::isfinite(maximum_)) {
            throw std::invalid_argument("interval bounds must be finite");
        }
        if (minimum_ > maximum_) {
            throw std::invalid_argument("minimum must not exceed maximum");
        }
    }

    [[nodiscard]] bool contains(double value) const noexcept
    {
        return value >= minimum_ && value <= maximum_;
    }

    [[nodiscard]] double length() const noexcept
    {
        return maximum_ - minimum_;
    }

    void shift(double delta)
    {
        if (!std::isfinite(delta)) {
            throw std::invalid_argument("shift must be finite");
        }

        const double newMinimum{minimum_ + delta};
        const double newMaximum{maximum_ + delta};
        if (!std::isfinite(newMinimum) || !std::isfinite(newMaximum)) {
            throw std::overflow_error("shifted interval is not finite");
        }

        minimum_ = newMinimum;
        maximum_ = newMaximum;
    }

    [[nodiscard]] double minimum() const noexcept { return minimum_; }
    [[nodiscard]] double maximum() const noexcept { return maximum_; }

private:
    double minimum_;
    double maximum_;
};

int main()
{
    ClosedInterval interval{-2.0, 3.0};
    std::cout << std::boolalpha << interval.contains(3.0) << '\n';
    interval.shift(10.0);
    std::cout << interval.minimum() << ' ' << interval.maximum() << '\n';
}
```

Инвариант `minimum <= maximum` устанавливается конструктором. Операция
`shift` меняет обе границы одинаково, поэтому порядок сохраняется. Новые
значения сначала вычисляются и проверяются локально, и только затем оба поля
обновляются.

У класса нет независимых `setMinimum` и `setMaximum`: они позволили бы
временно или постоянно нарушить договор.

## После главы 11: конфигурация с атомарным обновлением

```cpp
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdexcept>

class ModelConfig {
public:
    ModelConfig(double sampleRate, std::size_t frameCount)
        : sampleRate_{validateSampleRate(sampleRate)}
        , frameCount_{validateFrameCount(frameCount)}
    {
    }

    void update(double newSampleRate, std::size_t newFrameCount)
    {
        const double checkedRate{validateSampleRate(newSampleRate)};
        const std::size_t checkedCount{validateFrameCount(newFrameCount)};

        sampleRate_ = checkedRate;
        frameCount_ = checkedCount;
    }

    [[nodiscard]] double sampleRate() const noexcept { return sampleRate_; }
    [[nodiscard]] std::size_t frameCount() const noexcept { return frameCount_; }

private:
    static double validateSampleRate(double value)
    {
        if (!std::isfinite(value) || value <= 0.0) {
            throw std::invalid_argument("sample rate must be finite and positive");
        }
        return value;
    }

    static std::size_t validateFrameCount(std::size_t value)
    {
        if (value == 0) {
            throw std::invalid_argument("frame count must be positive");
        }
        return value;
    }

    double sampleRate_;
    std::size_t frameCount_;
};

int main()
{
    ModelConfig config{48'000.0, 10};

    try {
        config.update(96'000.0, 0);
    } catch (const std::invalid_argument& error) {
        std::cout << error.what() << '\n';
    }

    // Оба старых значения сохранились.
    std::cout << config.sampleRate() << ' ' << config.frameCount() << '\n';
}
```

Ключевой момент — оба аргумента проверяются до изменения полей. Если вторая
проверка бросит исключение, объект ещё не изменён. Два независимых сеттера не
могут выразить одну атомарную операцию так же ясно.

## После главы 12: расширение полиморфного пайплайна

Ниже показан компактный самодостаточный вариант с двумя обработчиками.

```cpp
#include <cmath>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class Processor {
public:
    virtual ~Processor() = default;
    virtual double apply(double value) const = 0;
};

class OffsetProcessor final : public Processor {
public:
    explicit OffsetProcessor(double offset) : offset_{offset} {}

    double apply(double value) const override
    {
        return value + offset_;
    }

private:
    double offset_;
};

class AbsoluteProcessor final : public Processor {
public:
    double apply(double value) const override
    {
        return std::abs(value);
    }
};

class Pipeline {
public:
    void add(std::unique_ptr<Processor> processor)
    {
        processors_.push_back(std::move(processor));
    }

    double apply(double value) const
    {
        for (const auto& processor : processors_) {
            value = processor->apply(value);
        }
        return value;
    }

private:
    std::vector<std::unique_ptr<Processor>> processors_;
};

int main()
{
    Pipeline pipeline;
    pipeline.add(std::make_unique<OffsetProcessor>(-5.0));
    pipeline.add(std::make_unique<AbsoluteProcessor>());

    std::cout << pipeline.apply(2.0) << '\n'; // abs(2 - 5) == 3
}
```

`Pipeline` — единственный владелец обработчиков. Метод `add` принимает
`unique_ptr` по значению, потому что вызов передаёт владение. Цикл использует
ссылку на сам `unique_ptr`, чтобы не копировать его — копирование уникального
владения запрещено.

Новый обработчик не требует изменения `Pipeline`. Но если набор операций
маленький, навсегда фиксирован и состоит только из данных, вариант с
`enum class` или `variant` может быть проще. Полиморфизм здесь является
учебным инструментом, а не обязательным ответом на любую задачу.

## Самостоятельное закрепление после разбора

Не перечитывая код выше:

1. Перепиши один пример с другими именами предметной области.
2. Добавь новый граничный случай.
3. Намеренно внеси ошибку контракта и убедись, что проверка её обнаруживает.
4. Объясни, где находятся значения, ссылки и владельцы.
5. Сравни свой первоначальный вариант с итоговым и запиши одно правило в
   журнал ошибок.
