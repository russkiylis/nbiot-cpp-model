# Краткая шпаргалка по учебнику

Шпаргалка напоминает форму конструкций, но не объясняет их полностью. Если
решение непонятно, вернись к соответствующей главе.

## Сборка CMake-проекта

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Для чистой проверки используй новый или очищенный отдельный каталог сборки.
Не удаляй каталоги, в назначении которых не уверен.

## Базовые объявления

```cpp
int count{0};
double gain{1.5};
bool enabled{true};
const double sampleRate{48'000.0};
std::string name{"signal"};
```

Фигурная инициализация помогает обнаруживать сужающие преобразования.

## Диапазоны индексов

Для контейнера размера `n` допустимы индексы:

```text
0, 1, ..., n - 1
```

Обычный цикл:

```cpp
for (std::size_t i{0}; i < values.size(); ++i) {
    // values[i]
}
```

Если индекс не нужен:

```cpp
for (const double value : values) {
    // использовать value
}
```

## Выбор способа передачи параметра

```cpp
void consumeSmallValue(int value);                    // маленькая копия
void inspect(const std::vector<double>& values);       // только чтение
void modify(std::vector<double>& values);              // изменение исходного
std::vector<double> createResult();                    // вернуть новое значение
```

Не применяй правило механически. Контракт важнее размера типа.

## Основные контейнеры

```cpp
std::array<double, 4> fixed{1.0, 2.0, 3.0, 4.0};
std::vector<double> dynamic{1.0, 2.0};

dynamic.push_back(3.0);
dynamic.reserve(1'000);

if (!dynamic.empty()) {
    const double first{dynamic.at(0)};
}
```

`array` имеет фиксированное число элементов. `vector` владеет непрерывным
динамическим буфером и может менять размер.

## Несколько алгоритмов STL

```cpp
#include <algorithm>
#include <numeric>

const double sum{
    std::accumulate(values.begin(), values.end(), 0.0)
};

const auto it{
    std::find_if(values.begin(), values.end(), [](double value) {
        return value < 0.0;
    })
};

std::sort(values.begin(), values.end());
```

Начальное значение `0.0`, а не `0`, помогает выполнять сумму в `double`.

## Простой класс с инвариантом

```cpp
class SampleRate {
public:
    explicit SampleRate(double value);

    [[nodiscard]] double value() const noexcept;

private:
    double value_;
};
```

Конструктор обязан не допустить неположительное или нечисловое значение, если
это входит в контракт.

## Владение

Задавай вопросы в таком порядке:

1. Можно ли хранить объект прямо по значению?
2. Есть ли один естественный владелец?
3. Нужен ли только временный доступ?
4. Действительно ли несколько частей управляют временем жизни совместно?

Типичные варианты:

```cpp
Widget widget;                         // локальное значение
Widget& view{widget};                  // невладеющая ссылка
Widget* optionalView{&widget};         // nullable, не обязательно владелец
auto owner{std::make_unique<Widget>()};
```

`shared_ptr` используй только при настоящем совместном владении.

## Перемещение `unique_ptr`

```cpp
auto source{std::make_unique<Widget>()};
auto destination{std::move(source)};

// destination владеет объектом
// source больше не владеет им
```

После перемещения проверяй только гарантированное состояние. Для
`unique_ptr` источник становится пустым.

## Полиморфный интерфейс

```cpp
class Operation {
public:
    virtual ~Operation() = default;
    virtual double apply(double value) const = 0;
};
```

Переопределение:

```cpp
class Gain final : public Operation {
public:
    explicit Gain(double factor) : factor_{factor} {}

    double apply(double value) const override
    {
        return value * factor_;
    }

private:
    double factor_;
};
```

Полиморфный базовый класс должен иметь виртуальный деструктор.

## Ошибки

Практическая схема выбора:

- `assert` — внутреннее предположение программиста;
- `optional` — отсутствие результата является обычным ожидаемым случаем;
- исключение — операция не может выполнить обещанный контракт и текущий
  уровень не может разумно продолжить;
- обычное возвращаемое значение — ошибка естественно является частью
  результата и явно представлена типом.

Не возвращай правдоподобный ноль вместо ошибки.

## Простой шаблон и concept

```cpp
#include <concepts>

template<std::floating_point T>
T square(T value)
{
    return value * value;
}
```

Делай код шаблонным только при реальной необходимости работать с семейством
типов.

## Floating point

Простое сравнение с абсолютным допуском:

```cpp
#include <cmath>

bool almostEqual(double left, double right, double tolerance)
{
    return std::abs(left - right) <= tolerance;
}
```

Для значений сильно разного масштаба одной абсолютной погрешности может быть
недостаточно; подробности разбираются в численной и тестовой главах.

## Перед переходом к следующей главе

- Код собирается.
- Обычный пример работает.
- Пустой или минимальный вход рассмотрен.
- Ошибочный параметр имеет определённое поведение.
- Ты можешь объяснить контракт.
- Ты можешь назвать владельца каждого объекта.
- Ты изменил пример, а не только повторил его.
