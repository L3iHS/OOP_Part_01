#include "array_ops.hpp"

#include <iostream>

int* array_create(std::size_t size)
{
    int* array{new int[size]{}};
    return array;
}

void array_delete(int*& arr)
{
    delete[] arr;
    arr = nullptr;
}

void array_print(const int* arr, std::size_t size)
{
    std::cout << '[';

    for (std::size_t i = 0; i < size; ++i)
    {
        std::cout << arr[i];

        if (i + 1 != size)
        {
            std::cout << ", ";
        }
    }

    std::cout << ']' << '\n';
}

int* array_resize(int* arr, std::size_t size, std::size_t new_size)
{
    int* new_arr{new int[new_size]{}};
    std::size_t copy_size{size < new_size ? size : new_size};

    for (std::size_t i = 0; i < copy_size; ++i)
    {
        new_arr[i] = arr[i];
    }

    delete[] arr;
    return new_arr;
}

int* array_insert(int* arr, std::size_t& size, std::size_t pos, int value)
{
    int* new_arr{new int[size + 1]{}};

    for (std::size_t i = 0; i < size; ++i)
    {
        if (i < pos)
        {
            new_arr[i] = arr[i];
        }
        else
        {
            new_arr[i + 1] = arr[i];
        }
    }

    new_arr[pos] = value;
    size++;
    delete[] arr;
    return new_arr;
}

int* array_remove(int* arr, std::size_t& size, std::size_t pos)
{
    if (size == 0 || pos >= size)
    {
        return arr;
    }

    int* new_arr{new int[size - 1]{}};

    for (std::size_t i = 0; i < size - 1; ++i)
    {
        if (i < pos)
        {
            new_arr[i] = arr[i];
        }
        else
        {
            new_arr[i] = arr[i + 1];
        }
    }

    size--;
    delete[] arr;
    return new_arr;
}