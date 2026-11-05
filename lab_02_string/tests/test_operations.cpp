#include "string.hpp"

#include <gtest/gtest.h>
#include <ostream>
#include <streambuf>
#include <utility>

namespace
{
class OutputBuffer : public std::streambuf
{
    char data_[128]{};

public:
    OutputBuffer() { setp(data_, data_ + sizeof(data_) - 1); }
    const char* data() const { return data_; }
    std::size_t size() const { return static_cast<std::size_t>(pptr() - pbase()); }
};
}  // namespace

TEST(StringPushBack, DoublesCapacityWhenBufferIsFull)
{
    String value;
    value.push_back('a');
    EXPECT_EQ(value.capacity(), 1U);
    value.push_back('b');
    EXPECT_EQ(value.capacity(), 2U);
    value.push_back('c');
    EXPECT_EQ(value.capacity(), 4U);
    const char* buffer = value.c_str();
    value.push_back('d');
    EXPECT_EQ(value.c_str(), buffer);
    EXPECT_EQ(value.size(), 4U);
    EXPECT_STREQ(value.c_str(), "abcd");
}

TEST(StringAppend, DoublesRepeatedlyUntilTextFits)
{
    String value("ab");
    value.append("cdefg");
    EXPECT_EQ(value.capacity(), 8U);
    EXPECT_EQ(value.size(), 7U);
    EXPECT_STREQ(value.c_str(), "abcdefg");
}

TEST(StringAppend, AppendsAnotherString)
{
    String value("hello");
    const String suffix(", world!");
    value.append(suffix);
    EXPECT_STREQ(value.c_str(), "hello, world!");
    EXPECT_STREQ(suffix.c_str(), ", world!");
}

TEST(StringAppend, EmptyAndNullInputsLeaveTextUnchanged)
{
    String value("hello");
    const String empty;
    const char* buffer = value.c_str();
    value.append("");
    value.append(nullptr);
    value.append(empty);
    EXPECT_STREQ(value.c_str(), "hello");
    EXPECT_EQ(value.c_str(), buffer);
}

TEST(StringAppend, HandlesSelfAppendWithReallocation)
{
    String value("ab");
    value.append(value);
    EXPECT_STREQ(value.c_str(), "abab");
    EXPECT_EQ(value.size(), 4U);
    EXPECT_EQ(value.capacity(), 4U);
}

TEST(StringAppend, HandlesSelfAppendWithoutReallocation)
{
    String value = String("aaaaab").remove_char('a');
    const char* buffer = value.c_str();
    value.append(value);
    EXPECT_STREQ(value.c_str(), "bb");
    EXPECT_EQ(value.c_str(), buffer);
}

TEST(StringAppend, HandlesPointerIntoItsOwnBuffer)
{
    String value("banana");
    value.append(value.c_str() + 2);
    EXPECT_STREQ(value.c_str(), "banananana");
    EXPECT_EQ(value.size(), 10U);
}

TEST(StringAppend, HandlesAliasedPointerWithoutReallocation)
{
    String value = String("aabbcc").remove_char('a');
    const char* buffer = value.c_str();
    value.append(value.c_str() + 3);
    EXPECT_STREQ(value.c_str(), "bbccc");
    EXPECT_EQ(value.c_str(), buffer);
}

TEST(StringFind, FindsFirstMiddleAndLastMatches)
{
    const String value("abcabc");
    EXPECT_EQ(value.find("abc"), 0U);
    EXPECT_EQ(value.find(String("bca")), 1U);
    EXPECT_EQ(value.find("cabc"), 2U);
    EXPECT_EQ(value.find("bc"), 1U);
    EXPECT_EQ(value.find("abcabc"), 0U);
}

TEST(StringFind, ReturnsNposForMissingOrLongerPattern)
{
    const String value("abc");
    EXPECT_EQ(value.find("x"), String::npos);
    EXPECT_EQ(value.find("abcd"), String::npos);
    EXPECT_EQ(String().find("a"), String::npos);
}

TEST(StringFind, EmptyPatternMatchesAtZero)
{
    EXPECT_EQ(String("abc").find(""), 0U);
    EXPECT_EQ(String().find(""), 0U);
    EXPECT_EQ(String("abc").find(nullptr), 0U);
}

TEST(StringPrint, WritesTextWithoutExtraCharacters)
{
    OutputBuffer buffer;
    std::ostream output(&buffer);
    String("hello, world!").print(output);
    EXPECT_EQ(buffer.size(), 13U);
    EXPECT_STREQ(buffer.data(), "hello, world!");
}

TEST(StringPrint, WritesNothingForEmptyOrMovedFromString)
{
    OutputBuffer buffer;
    std::ostream output(&buffer);
    String original("text");
    const String moved(std::move(original));
    original.print(output);
    String().print(output);
    EXPECT_EQ(buffer.size(), 0U);
    EXPECT_EQ(original.find("text"), String::npos);
    EXPECT_STREQ(original.remove_char('x').c_str(), "");
}

TEST(StringRemoveChar, MatchesVariantExampleAndPreservesSource)
{
    const String source("banana");
    const String result = source.remove_char('a');
    EXPECT_STREQ(result.c_str(), "bnn");
    EXPECT_EQ(result.size(), 3U);
    EXPECT_STREQ(source.c_str(), "banana");
    EXPECT_NE(source.c_str(), result.c_str());
}

TEST(StringRemoveChar, MissingCharacterPreservesContents)
{
    const String result = String("abc").remove_char('x');
    EXPECT_STREQ(result.c_str(), "abc");
    EXPECT_EQ(result.size(), 3U);
}

TEST(StringRemoveChar, RemovingAllCharactersProducesEmptyString)
{
    const String result = String("aaaa").remove_char('a');
    EXPECT_EQ(result.size(), 0U);
    EXPECT_STREQ(result.c_str(), "");
}

TEST(StringRemoveChar, HandlesEmptyAndSingleCharacter)
{
    EXPECT_STREQ(String().remove_char('a').c_str(), "");
    EXPECT_STREQ(String("a").remove_char('a').c_str(), "");
    EXPECT_STREQ(String("b").remove_char('a').c_str(), "b");
}

TEST(StringRemoveChar, PreservesOrderWithAdjacentAndEdgeMatches)
{
    const String result = String("aabaacadaa").remove_char('a');
    EXPECT_STREQ(result.c_str(), "bcd");
    EXPECT_EQ(result.size(), 3U);
}

TEST(StringBytes, PreservesEmbeddedNullWhenCopyingAppendingAndPrinting)
{
    String value("a");
    value.push_back('\0');
    value.push_back('b');
    String copy(value);
    EXPECT_EQ(copy.size(), 3U);
    EXPECT_EQ(copy.c_str()[2], 'b');
    copy.append(value);
    EXPECT_EQ(copy.size(), 6U);
    EXPECT_EQ(copy.find(value), 0U);
    EXPECT_EQ(copy.c_str()[5], 'b');

    OutputBuffer buffer;
    std::ostream output(&buffer);
    value.print(output);
    EXPECT_EQ(buffer.size(), 3U);
    EXPECT_EQ(buffer.data()[1], '\0');
    EXPECT_EQ(buffer.data()[2], 'b');
    EXPECT_STREQ(value.remove_char('\0').c_str(), "ab");
}
