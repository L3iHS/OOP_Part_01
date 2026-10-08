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

TEST(ArrayRemoveAll, MatchesAssignmentExample)
{
    const int values[]{2, 5, 2, 8, 2, 1};
    std::size_t size{6};
    int* arr{make_array(values, size)};
    int* original{arr};
    EXPECT_EQ(array_remove_all(arr, size, 2), 3U);
    const int expected[]{5, 8, 1};
    expect_array(arr, size, expected, 3);
    EXPECT_EQ(arr, original);
    array_delete(arr);
}

TEST(ArrayRemoveAll, AllMatchesProduceEmptyArray)
{
    const int values[]{2, 2, 2};
    std::size_t size{3};
    int* arr{make_array(values, size)};
    EXPECT_EQ(array_remove_all(arr, size, 2), 3U);
    EXPECT_EQ(size, 0U);
    array_delete(arr);
}

TEST(ArrayRemoveAll, MissingValueLeavesArrayUnchanged)
{
    const int values[]{-1, 0, 4};
    std::size_t size{3};
    int* arr{make_array(values, size)};
    EXPECT_EQ(array_remove_all(arr, size, 2), 0U);
    expect_array(arr, size, values, 3);
    array_delete(arr);
}

TEST(ArrayGnomeSort, HandlesDuplicatesAndNegativeValues)
{
    const int values[]{3, -2, 3, 0, -2};
    int* arr{make_array(values, 5)};
    array_gnome_sort(arr, 5);
    const int expected[]{-2, -2, 0, 3, 3};
    expect_array(arr, 5, expected, 5);
    array_delete(arr);
}

TEST(ArrayBinarySearch, FindsExistingValuesAndTheirIndices)
{
    const int values[]{1, 3, 5, 7};
    int* arr{make_array(values, 4)};
    std::size_t index{99};
    EXPECT_TRUE(array_binary_search(arr, 4, 1, index));
    EXPECT_EQ(index, 0U);
    EXPECT_TRUE(array_binary_search(arr, 4, 5, index));
    EXPECT_EQ(index, 2U);
    EXPECT_TRUE(array_binary_search(arr, 4, 7, index));
    EXPECT_EQ(index, 3U);
    array_delete(arr);
}

TEST(ArrayBinarySearch, MissingValueBetweenElementsReturnsFalse)
{
    const int values[]{1, 3, 5, 7};
    int* arr{make_array(values, 4)};
    std::size_t index{};
    EXPECT_FALSE(array_binary_search(arr, 4, 4, index));
    array_delete(arr);
}

TEST(EmptyArray, AlgorithmsAndSearchAreSafe)
{
    int* arr{nullptr};
    std::size_t size{};
    std::size_t index{};

    array_gnome_sort(arr, size);
    EXPECT_EQ(array_remove_all(arr, size, 2), 0U);
    EXPECT_FALSE(array_binary_search(arr, size, 2, index));
    EXPECT_EQ(size, 0U);

    arr = array_create(0);
    array_gnome_sort(arr, size);
    EXPECT_EQ(array_remove_all(arr, size, 2), 0U);
    EXPECT_FALSE(array_binary_search(arr, size, 2, index));
    EXPECT_EQ(size, 0U);
    array_delete(arr);
}
