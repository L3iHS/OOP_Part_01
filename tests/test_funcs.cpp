#include "array_ops.hpp"

#include <gtest/gtest.h>

namespace
{
int* make_array(const int* values, std::size_t size)
{
    int* arr{array_create(size)};
    for (std::size_t i = 0; i < size; ++i)
    {
        arr[i] = values[i];
    }
    return arr;
}

void expect_array(const int* arr, std::size_t size, const int* expected,
                  std::size_t expected_size)
{
    ASSERT_EQ(size, expected_size);
    if (size != 0)
    {
        ASSERT_NE(arr, nullptr);
    }
    for (std::size_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(arr[i], expected[i]) << "Index: " << i;
    }
}
}  // namespace

TEST(ArrayCreate, InitializesElementsToZero)
{
    int* arr{array_create(3)};
    const int expected[]{0, 0, 0};
    expect_array(arr, 3, expected, 3);
    array_delete(arr);
}

TEST(ArrayDelete, ResetsCallerPointer)
{
    int* arr{array_create(3)};
    array_delete(arr);
    EXPECT_EQ(arr, nullptr);
}

TEST(ArrayResize, GrowthPreservesValuesAndAddsZeros)
{
    const int values[]{4, -2, 9};
    int* arr{make_array(values, 3)};
    arr = array_resize(arr, 3, 5);
    const int expected[]{4, -2, 9, 0, 0};
    expect_array(arr, 5, expected, 5);
    array_delete(arr);
}

TEST(ArrayResize, ShrinkingPreservesPrefix)
{
    const int values[]{4, -2, 9, 8};
    int* arr{make_array(values, 4)};
    arr = array_resize(arr, 4, 2);
    const int expected[]{4, -2};
    expect_array(arr, 2, expected, 2);
    array_delete(arr);
}

TEST(ArrayInsert, InsertsInMiddle)
{
    const int values[]{10, 20, 30};
    std::size_t size{3};
    int* arr{make_array(values, size)};
    arr = array_insert(arr, size, 1, 15);
    const int expected[]{10, 15, 20, 30};
    expect_array(arr, size, expected, 4);
    array_delete(arr);
}

TEST(ArrayInsert, InsertsIntoEmptyArray)
{
    int* arr{nullptr};
    std::size_t size{};
    arr = array_insert(arr, size, 0, -7);
    const int expected[]{-7};
    expect_array(arr, size, expected, 1);
    array_delete(arr);
}

TEST(ArrayRemove, RemovesMiddleElement)
{
    const int values[]{10, 20, 30};
    std::size_t size{3};
    int* arr{make_array(values, size)};
    arr = array_remove(arr, size, 1);
    const int expected[]{10, 30};
    expect_array(arr, size, expected, 2);
    array_delete(arr);
}

TEST(ArrayRemove, EmptyArrayRemainsEmpty)
{
    int* arr{nullptr};
    std::size_t size{};
    arr = array_remove(arr, size, 0);
    EXPECT_EQ(size, 0U);
    EXPECT_EQ(arr, nullptr);
    array_delete(arr);
}
