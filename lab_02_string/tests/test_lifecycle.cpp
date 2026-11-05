#include "string.hpp"

#include <gtest/gtest.h>
#include <utility>

namespace
{
void expect_moved_from(const String& value)
{
    EXPECT_EQ(value.size(), 0U);
    EXPECT_EQ(value.capacity(), 0U);
    ASSERT_NE(value.c_str(), nullptr);
    EXPECT_STREQ(value.c_str(), "");
}
}  // namespace

TEST(StringCopy, OwnsIndependentBuffer)
{
    String original("hello");
    const String copy(original);
    ASSERT_NE(copy.c_str(), original.c_str());
    original.push_back('!');
    EXPECT_STREQ(copy.c_str(), "hello");
    EXPECT_STREQ(original.c_str(), "hello!");
    EXPECT_EQ(copy.size(), 5U);
}

TEST(StringCopy, CopiesEmptyString)
{
    const String original;
    const String copy(original);
    EXPECT_STREQ(copy.c_str(), "");
    EXPECT_EQ(copy.size(), 0U);
    EXPECT_GE(copy.capacity(), 1U);
    EXPECT_NE(copy.c_str(), original.c_str());
}

TEST(StringMove, TransfersBufferAndClearsSource)
{
    String original("hello");
    const char* buffer = original.c_str();
    const String moved(std::move(original));
    EXPECT_EQ(moved.c_str(), buffer);
    EXPECT_EQ(moved.size(), 5U);
    EXPECT_STREQ(moved.c_str(), "hello");
    expect_moved_from(original);
}

TEST(StringMove, MovesEmptyString)
{
    String original;
    const char* buffer = original.c_str();
    const String moved(std::move(original));
    EXPECT_EQ(moved.c_str(), buffer);
    EXPECT_STREQ(moved.c_str(), "");
    expect_moved_from(original);
}

TEST(StringMove, SourceCanBeUsedAgain)
{
    String original("old");
    const String moved(std::move(original));
    original.push_back('n');
    original.append("ew");
    EXPECT_STREQ(original.c_str(), "new");
    EXPECT_STREQ(moved.c_str(), "old");
}

TEST(StringCopy, CopiesMovedFromObject)
{
    String original("text");
    const String moved(std::move(original));
    const String copy(original);
    EXPECT_EQ(copy.size(), 0U);
    EXPECT_GE(copy.capacity(), 1U);
    EXPECT_STREQ(copy.c_str(), "");
}

TEST(StringAssignment, CopiesIntoNonemptyObject)
{
    const String source("new");
    String destination("previous text");
    String& result = (destination = source);
    EXPECT_EQ(&result, &destination);
    EXPECT_STREQ(destination.c_str(), "new");
    EXPECT_NE(destination.c_str(), source.c_str());
}

TEST(StringAssignment, AssignsEmptyObject)
{
    String destination("previous text");
    destination = String();
    EXPECT_EQ(destination.size(), 0U);
    EXPECT_STREQ(destination.c_str(), "");
}

TEST(StringAssignment, HandlesSelfCopy)
{
    String value("hello");
    const String& alias = value;
    value = alias;
    EXPECT_STREQ(value.c_str(), "hello");
    EXPECT_EQ(value.size(), 5U);
}

TEST(StringAssignment, MovesIntoNonemptyObject)
{
    String source("new");
    const char* buffer = source.c_str();
    String destination("previous text");
    destination = std::move(source);
    EXPECT_EQ(destination.c_str(), buffer);
    EXPECT_STREQ(destination.c_str(), "new");
    expect_moved_from(source);
}

TEST(StringAssignment, HandlesSelfMove)
{
    String value("hello");
    String& alias = value;
    value = std::move(alias);
    EXPECT_STREQ(value.c_str(), "hello");
    EXPECT_EQ(value.size(), 5U);
}

TEST(StringSwap, ExchangesBuffersAndHandlesItself)
{
    String first("one");
    String second("second");
    const char* first_buffer = first.c_str();
    const char* second_buffer = second.c_str();
    first.swap(second);
    EXPECT_EQ(first.c_str(), second_buffer);
    EXPECT_EQ(second.c_str(), first_buffer);
    EXPECT_EQ(first.size(), 6U);
    EXPECT_EQ(second.size(), 3U);
    first.swap(first);
    EXPECT_STREQ(first.c_str(), "second");
}
