#include <iostream>
#include <limits>
#include <new>

#include "src/array_ops.hpp"

namespace
{
// false означает завершение ввода; ошибочное число можно ввести повторно.
bool read_int(const char* prompt, int& value)
{
    while (true)
    {
        std::cout << prompt;

        if (std::cin >> value)
        {
            return true;
        }

        if (std::cin.eof() || std::cin.bad())
        {
            return false;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка ввода — введите целое число\n";
    }
}

bool read_size(const char* prompt, std::size_t& value)
{
    int requested{};

    while (read_int(prompt, requested))
    {
        if (requested >= 0)
        {
            value = static_cast<std::size_t>(requested);
            return true;
        }

        std::cout << "Размер и индекс должны быть неотрицательными\n";
    }

    return false;
}

void print_menu()
{
    std::cout << "\n1. Создать массив\n"
                 "2. Напечатать массив\n"
                 "3. Вставить элемент\n"
                 "4. Удалить элемент по индексу\n"
                 "5. Изменить размер\n"
                 "6. Удалить все вхождения числа\n"
                 "7. Гномья сортировка по возрастанию\n"
                 "8. Бинарный поиск\n"
                 "0. Выход\n";
}

void print_array_state(const int* arr, std::size_t size)
{
    std::cout << "Размер массива: " << size << '\n';
    array_print(arr, size);
}

bool is_sorted(const int* arr, std::size_t size)
{
    for (std::size_t i = 1; i < size; ++i)
    {
        if (arr[i - 1] > arr[i])
        {
            return false;
        }
    }

    return true;
}
}  // namespace

int main()
{
    int* arr{nullptr};
    std::size_t size{};
    bool running{true};

    while (running)
    {
        print_menu();

        int command{};
        if (!read_int("Выберите пункт: ", command))
        {
            break;
        }

        try
        {
            switch (command)
            {
                case 0:
                    running = false;
                    break;

                case 1:
                {
                    std::size_t new_size{};
                    if (!read_size("Введите размер массива: ", new_size))
                    {
                        running = false;
                        break;
                    }

                    // Прежний массив остаётся доступен, пока новый не заполнен.
                    int* new_arr{array_create(new_size)};
                    bool complete{true};

                    for (std::size_t i = 0; i < new_size; ++i)
                    {
                        std::cout << "Элемент [" << i << "]: ";
                        if (!read_int("", new_arr[i]))
                        {
                            complete = false;
                            break;
                        }
                    }

                    if (!complete)
                    {
                        array_delete(new_arr);
                        running = false;
                        break;
                    }

                    array_delete(arr);
                    arr = new_arr;
                    size = new_size;
                    print_array_state(arr, size);
                    break;
                }

                case 2:
                    print_array_state(arr, size);
                    break;

                case 3:
                {
                    std::size_t pos{};
                    if (!read_size("Индекс вставки (от 0 до размера включительно): ", pos))
                    {
                        running = false;
                        break;
                    }

                    if (pos > size)
                    {
                        std::cout << "Индекс вставки не должен превышать размер массива\n";
                        break;
                    }

                    int value{};
                    if (!read_int("Введите значение: ", value))
                    {
                        running = false;
                        break;
                    }

                    arr = array_insert(arr, size, pos, value);
                    print_array_state(arr, size);
                    break;
                }

                case 4:
                {
                    if (size == 0)
                    {
                        std::cout << "Массив пуст: удалять нечего\n";
                        break;
                    }

                    std::size_t pos{};
                    if (!read_size("Введите индекс удаляемого элемента: ", pos))
                    {
                        running = false;
                        break;
                    }

                    if (pos >= size)
                    {
                        std::cout << "Индекс должен быть меньше размера массива\n";
                        break;
                    }

                    arr = array_remove(arr, size, pos);
                    print_array_state(arr, size);
                    break;
                }

                case 5:
                {
                    std::size_t new_size{};
                    if (!read_size("Введите новый размер: ", new_size))
                    {
                        running = false;
                        break;
                    }

                    arr = array_resize(arr, size, new_size);
                    size = new_size;
                    print_array_state(arr, size);
                    break;
                }

                case 6:
                {
                    int value{};
                    if (!read_int("Введите число для удаления всех вхождений: ", value))
                    {
                        running = false;
                        break;
                    }

                    std::size_t removed_count{array_remove_all(arr, size, value)};
                    std::cout << "Удалено элементов: " << removed_count << '\n';
                    print_array_state(arr, size);
                    break;
                }

                case 7:
                    array_gnome_sort(arr, size);
                    std::cout << "Массив отсортирован по возрастанию\n";
                    print_array_state(arr, size);
                    break;

                case 8:
                {
                    if (size == 0)
                    {
                        std::cout << "Массив пуст — значение не найдено\n";
                        break;
                    }

                    if (!is_sorted(arr, size))
                    {
                        std::cout << "Для бинарного поиска сначала отсортируйте массив (пункт 7)\n";
                        break;
                    }

                    int target{};
                    if (!read_int("Введите искомое число: ", target))
                    {
                        running = false;
                        break;
                    }

                    std::size_t index{};
                    if (array_binary_search(arr, size, target, index))
                    {
                        std::cout << "Найдено значение " << arr[index] << " по индексу " << index << '\n';
                    }
                    else
                    {
                        std::cout << "Значение не найдено\n";
                    }

                    break;
                }

                default:
                    std::cout << "Неизвестный пункт меню\n";
                    break;
            }
        }
        catch (const std::bad_alloc&)
        {
            std::cerr << "Не удалось выделить память — прежний массив сохранён\n";
        }
    }

    array_delete(arr);
    std::cout << "Программа завершена\n";
    return 0;
}
