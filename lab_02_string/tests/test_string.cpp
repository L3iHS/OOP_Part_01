#include "string.hpp"

#include <gtest/gtest.h>

namespace
{
void expect_string(const String& value, const char* expected, std::size_t size)
{
    ASSERT_NE(value.c_str(), nullptr);
    ASSERT_EQ(value.size(), size);
    EXPECT_GE(value.capacity(), size);
    EXPECT_GE(value.capacity(), 1U);
    EXPECT_STREQ(value.c_str(), expected);
    EXPECT_EQ(value.c_str()[size], '\0');
}
}  // namespace

TEST(StringConstruction, DefaultCreatesEmptyString)
{
    const String value;
    expect_string(value, "", 0);
    EXPECT_EQ(value.capacity(), 1U);
}

TEST(StringConstruction, NullPointerCreatesEmptyString)
{
    const String value(nullptr);
    expect_string(value, "", 0);
    EXPECT_EQ(value.capacity(), 1U);
}

TEST(StringConstruction, EmptyTextCreatesEmptyString)
{
    const String value("");
    expect_string(value, "", 0);
    EXPECT_EQ(value.capacity(), 1U);
}

TEST(StringConstruction, PreservesSingleCharacter)
{
    const String value("x");
    expect_string(value, "x", 1);
    EXPECT_EQ(value.capacity(), 1U);
}

TEST(StringConstruction, CopiesOrdinaryText)
{
    const String value("banana");
    expect_string(value, "banana", 6);
}

TEST(StringConstruction, PreservesSpacesAndPunctuation)
{
    const String value("hello, world!");
    expect_string(value, "hello, world!", 13);
}

TEST(StringConstruction, PreservesTabsAndNewlines)
{
    const String value("a\tb\n");
    expect_string(value, "a\tb\n", 4);
}

TEST(StringConstruction, StopsAtFirstNullCharacter)
{
    const char source[]{'a', 'b', '\0', 'x', '\0'};
    const String value(source);
    expect_string(value, "ab", 2);
}

TEST(StringOwnership, SourceChangesDoNotAffectString)
{
    char source[]{"hello"};
    const String value(source);
    source[0] = 'X';
    source[1] = '\0';
    expect_string(value, "hello", 5);
}

TEST(StringOwnership, SourceCanBeReleasedAfterConstruction)
{
    char* source{new char[4]{'c', 'a', 't', '\0'}};
    const String value(source);
    delete[] source;
    expect_string(value, "cat", 3);
}

TEST(StringOwnership, SeparateObjectsOwnSeparateBuffers)
{
    const String first("same");
    const String second("same");
    EXPECT_NE(first.c_str(), second.c_str());
    expect_string(first, "same", 4);
    expect_string(second, "same", 4);
}

TEST(StringConstruction, CopiesLongText)
{
    constexpr std::size_t size{4096};
    char source[size + 1]{};
    for (std::size_t i = 0; i < size; ++i)
    {
        source[i] = static_cast<char>('a' + i % 26);
    }

    const String value(source);
    expect_string(value, source, size);
}
