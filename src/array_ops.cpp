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

std::size_t array_remove_all(int* arr, std::size_t& size, int value)
{
    std::size_t write_index{};

    for (std::size_t read_index = 0; read_index < size; ++read_index)
    {
        if (arr[read_index] == value)
        {
            continue;
        }

        arr[write_index] = arr[read_index];
        write_index++;
    }

    std::size_t removed_count{};
    removed_count = size - write_index;
    size = write_index;

    return removed_count;
}

void array_gnome_sort(int* arr, std::size_t size)
{
    std::size_t i{};

    while (i < size)
    {
        if (i == 0 || arr[i - 1] <= arr[i])
        {
            i++;
            continue;
        }

        int t{arr[i - 1]};
        arr[i - 1] = arr[i];
        arr[i] = t;
        i--;
    }
}

bool array_binary_search(const int* arr, std::size_t size, int target, std::size_t& out_index)
{
    std::size_t left{};
    std::size_t right = size;

    while (left < right)
    {
        std::size_t mid{left + (right - left) / 2};

        if (target < arr[mid])
        {
            right = mid;
            mid = left + (right - left) / 2;
            continue;
        }
        else if (target > arr[mid])
        {
            left = mid + 1;
            mid = left + (right - left) / 2;
            continue;
        }
        else
        {
            out_index = mid;
            return true;
        }
    }
    return false;
}