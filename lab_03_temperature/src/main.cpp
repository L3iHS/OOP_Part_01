#include "temperature_set.hpp"

#include <iostream>

namespace
{
void discard_line()
{
    char ch{};
    while (std::cin.get(ch) && ch != '\n')
    {
    }
}

bool read_temperature(Temperature& temperature)
{
    using Unit = Temperature::Unit;

    std::cout << "Введите значение и единицу (C, F или K): ";
    double value{};
    char symbol{};
    if (!(std::cin >> value >> symbol))
    {
        if (!std::cin.eof())
        {
            std::cin.clear();
            discard_line();
            std::cout << "Ожидается число и единица измерения\n";
        }
        return false;
    }
    discard_line();

    Unit unit{};
    switch (symbol)
    {
    case 'C':
    case 'c':
        unit = Unit::Celsius;
        break;
    case 'F':
    case 'f':
        unit = Unit::Fahrenheit;
        break;
    case 'K':
    case 'k':
        unit = Unit::Kelvin;
        break;
    default:
        std::cout << "Допустимые единицы: C, F, K\n";
        return false;
    }

    temperature = Temperature(value, unit);
    if (!temperature.is_valid())
    {
        std::cout << "Введите конечное число\n";
        return false;
    }
    return true;
}
}

int main()
{
    TemperatureSet first;
    TemperatureSet second;
    bool use_second{false};

    while (std::cin)
    {
        std::cout << "\nТекущее множество: " << (use_second ? 'B' : 'A') << '\n';
        std::cout << "1. Переключить множество\n"
                     "2. Добавить температуру\n"
                     "3. Удалить температуру\n"
                     "4. Найти температуру\n"
                     "5. Показать множества\n"
                     "6. Показать объединение, пересечение и разность\n"
                     "0. Выход\n"
                     "Выбор: ";

        int choice{};
        if (!(std::cin >> choice))
        {
            if (std::cin.eof())
            {
                break;
            }
            std::cin.clear();
            discard_line();
            std::cout << "Введите номер пункта\n";
            continue;
        }
        discard_line();

        TemperatureSet& current{use_second ? second : first};
        switch (choice)
        {
        case 0:
            return 0;
        case 1:
            use_second = !use_second;
            break;
        case 2:
        case 3:
        case 4:
        {
            Temperature temperature;
            if (!read_temperature(temperature))
            {
                break;
            }
            if (choice == 2)
            {
                std::cout << (current.add(temperature) ? "Температура добавлена\n"
                                                       : "Такая температура уже есть\n");
            }
            else if (choice == 3)
            {
                std::cout << (current.remove(temperature) ? "Температура удалена\n"
                                                          : "Температура отсутствует\n");
            }
            else
            {
                std::cout << (current(temperature) ? "Температура найдена\n"
                                                   : "Температура отсутствует\n");
            }
            break;
        }
        case 5:
            std::cout << "A = " << first << " (элементов: " << first.size() << ")\n"
                      << "B = " << second << " (элементов: " << second.size() << ")\n"
                      << (first == second ? "Множества равны\n" : "Множества различаются\n");
            break;
        case 6:
            std::cout << "A | B = " << (first | second) << '\n'
                      << "A & B = " << (first & second) << '\n'
                      << "A - B = " << (first - second) << '\n';
            break;
        default:
            std::cout << "Нет такого пункта\n";
        }
    }

    return 0;
}
