#include "array_ops.hpp"

#include <gtest/gtest.h>

#include <limits>

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

TEST(ArrayRemoveAll, RemovesAdjacentMatches)
{
    const int values[]{2, 2, 5, 2, 2, 8, 2};
    std::size_t size{7};
    int* arr{make_array(values, size)};
    EXPECT_EQ(array_remove_all(arr, size, 2), 5U);
    const int expected[]{5, 8};
    expect_array(arr, size, expected, 2);
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

TEST(ArrayRemoveAll, EmptyArrayHasNoMatches)
{
    int* arr{nullptr};
    std::size_t size{};
    EXPECT_EQ(array_remove_all(arr, size, 2), 0U);
    EXPECT_EQ(size, 0U);
    EXPECT_EQ(arr, nullptr);
    array_delete(arr);
}

TEST(ArrayRemoveAll, ArrayCanBeUsedAfterCompaction)
{
    const int values[]{2, 5, 2, 8};
    std::size_t size{4};
    int* arr{make_array(values, size)};
    EXPECT_EQ(array_remove_all(arr, size, 2), 2U);
    arr = array_insert(arr, size, 1, 7);
    const int expected[]{5, 7, 8};
    expect_array(arr, size, expected, 3);
    array_delete(arr);
}

TEST(ArrayGnomeSort, SortsReverseOrder)
{
    const int values[]{5, 4, 3, 2, 1};
    int* arr{make_array(values, 5)};
    array_gnome_sort(arr, 5);
    const int expected[]{1, 2, 3, 4, 5};
    expect_array(arr, 5, expected, 5);
    array_delete(arr);
}

TEST(ArrayGnomeSort, PreservesSortedArray)
{
    const int values[]{1, 2, 3, 4};
    int* arr{make_array(values, 4)};
    array_gnome_sort(arr, 4);
    expect_array(arr, 4, values, 4);
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

TEST(ArrayGnomeSort, AcceptsEmptyArray)
{
    int* arr{nullptr};
    array_gnome_sort(arr, 0);
    EXPECT_EQ(arr, nullptr);
    array_delete(arr);
}

TEST(ArrayGnomeSort, PreservesSingleElement)
{
    const int values[]{-7};
    int* arr{make_array(values, 1)};
    array_gnome_sort(arr, 1);
    expect_array(arr, 1, values, 1);
    array_delete(arr);
}

TEST(ArrayGnomeSort, HandlesExtremeIntValues)
{
    const int minimum{std::numeric_limits<int>::min()};
    const int maximum{std::numeric_limits<int>::max()};
    const int values[]{maximum, 0, minimum};
    int* arr{make_array(values, 3)};
    array_gnome_sort(arr, 3);
    const int expected[]{minimum, 0, maximum};
    expect_array(arr, 3, expected, 3);
    array_delete(arr);
}

TEST(ArrayBinarySearch, FindsMiddleElementInOddSizeArray)
{
    const int values[]{1, 3, 5, 7, 9};
    int* arr{make_array(values, 5)};
    std::size_t index{99};
    EXPECT_TRUE(array_binary_search(arr, 5, 5, index));
    EXPECT_EQ(index, 2U);
    array_delete(arr);
}

TEST(ArrayBinarySearch, FindsFirstElementInEvenSizeArray)
{
    const int values[]{1, 3, 5, 7};
    int* arr{make_array(values, 4)};
    std::size_t index{99};
    EXPECT_TRUE(array_binary_search(arr, 4, 1, index));
    EXPECT_EQ(index, 0U);
    array_delete(arr);
}

TEST(ArrayBinarySearch, FindsLastElement)
{
    const int values[]{1, 3, 5, 7};
    int* arr{make_array(values, 4)};
    std::size_t index{99};
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

TEST(ArrayBinarySearch, ValuesOutsideRangeReturnFalse)
{
    const int values[]{1, 3, 5, 7};
    int* arr{make_array(values, 4)};
    std::size_t index{};
    EXPECT_FALSE(array_binary_search(arr, 4, -10, index));
    EXPECT_FALSE(array_binary_search(arr, 4, 10, index));
    array_delete(arr);
}

TEST(ArrayBinarySearch, EmptyArrayReturnsFalse)
{
    std::size_t index{};
    EXPECT_FALSE(array_binary_search(nullptr, 0, 5, index));
}

TEST(ArrayBinarySearch, HandlesSingleElement)
{
    const int values[]{-7};
    int* arr{make_array(values, 1)};
    std::size_t index{99};
    EXPECT_TRUE(array_binary_search(arr, 1, -7, index));
    EXPECT_EQ(index, 0U);
    EXPECT_FALSE(array_binary_search(arr, 1, 0, index));
    array_delete(arr);
}

TEST(ArrayBinarySearch, FindsOneOfDuplicateValues)
{
    const int values[]{1, 2, 2, 2, 5};
    int* arr{make_array(values, 5)};
    std::size_t index{99};
    const bool found{array_binary_search(arr, 5, 2, index)};
    EXPECT_TRUE(found);
    EXPECT_LT(index, 5U);
    if (found && index < 5)
    {
        EXPECT_EQ(arr[index], 2);
    }
    array_delete(arr);
}

TEST(Variant4, SortSearchAndRemoveAllWorkTogether)
{
    const int values[]{2, 5, 2, 8, 2, 1};
    std::size_t size{6};
    int* arr{make_array(values, size)};

    array_gnome_sort(arr, size);
    const int sorted[]{1, 2, 2, 2, 5, 8};
    expect_array(arr, size, sorted, 6);

    std::size_t index{};
    EXPECT_TRUE(array_binary_search(arr, size, 8, index));
    EXPECT_EQ(index, 5U);

    EXPECT_EQ(array_remove_all(arr, size, 2), 3U);
    const int remaining[]{1, 5, 8};
    expect_array(arr, size, remaining, 3);
    EXPECT_FALSE(array_binary_search(arr, size, 2, index));
    EXPECT_TRUE(array_binary_search(arr, size, 8, index));
    EXPECT_EQ(index, 2U);

    array_delete(arr);
}
