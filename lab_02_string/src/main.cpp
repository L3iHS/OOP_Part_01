#include "string.hpp"

#include <iostream>
#include <exception>
#include <limits>
#include <utility>

namespace
{
String read_line()
{
    String line;
    char c{};
    while (std::cin.get(c) && c != '\n')
    {
        line.push_back(c);
    }
    return line;
}

bool read_character(char& c)
{
    const String line = read_line();
    if (line.size() != 1)
    {
        std::cout << "Введите один символ\n";
        return false;
    }
    c = line.c_str()[0];
    return true;
}

void show_string(const String& text)
{
    std::cout << "Строка: ";
    text.print(std::cout);
    std::cout << "\nДлина: " << text.size();
    std::cout << "\nЁмкость: " << text.capacity() << '\n';
}

void demonstrate_copy_and_move(const String& text)
{
    String copy(text);
    copy.push_back('!');
    String moved(std::move(copy));

    std::cout << "Исходная строка: ";
    text.print(std::cout);
    std::cout << "\nКопия после перемещения: ";
    copy.print(std::cout);
    std::cout << "\nНовый владелец буфера: ";
    moved.print(std::cout);
    std::cout << '\n';
}
}  // namespace

int main()
{
    String text;
    while (true)
    {
        std::cout << "\n1. Ввести строку\n"
                     "2. Показать строку, длину и ёмкость\n"
                     "3. Добавить символ\n"
                     "4. Дописать строку\n"
                     "5. Найти подстроку\n"
                     "6. Удалить символ в копии\n"
                     "7. Показать копирование и перемещение\n"
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Введите номер пункта\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 0)
        {
            break;
        }

        try
        {
            switch (choice)
            {
            case 1:
                std::cout << "Строка: ";
                text = read_line();
                break;
            case 2:
                show_string(text);
                break;
            case 3:
            {
                std::cout << "Символ: ";
                char c{};
                if (read_character(c))
                {
                    text.push_back(c);
                    show_string(text);
                }
                break;
            }
            case 4:
                std::cout << "Дополнение: ";
                text.append(read_line());
                show_string(text);
                break;
            case 5:
            {
                std::cout << "Подстрока: ";
                const std::size_t pos{text.find(read_line())};
                if (pos == String::npos)
                {
                    std::cout << "Подстрока не найдена\n";
                }
                else
                {
                    std::cout << "Позиция: " << pos << '\n';
                }
                break;
            }
            case 6:
            {
                std::cout << "Удаляемый символ: ";
                char c{};
                if (read_character(c))
                {
                    const String result = text.remove_char(c);
                    std::cout << "Результат: ";
                    result.print(std::cout);
                    std::cout << '\n';
                }
                break;
            }
            case 7:
                demonstrate_copy_and_move(text);
                break;
            default:
                std::cout << "Нет такого пункта\n";
            }
        }
        catch (const std::exception& error)
        {
            std::cerr << "Ошибка: " << error.what() << '\n';
        }
    }

    return 0;
}
