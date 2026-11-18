#include "temperature_set.hpp"

#include <gtest/gtest.h>
#include <utility>

TEST(TemperatureSetLifecycle, CopyConstructorMakesIndependentCopy)
{
    TemperatureSet source;
    source += Temperature(10);
    source += Temperature(20);

    TemperatureSet copy(source);
    EXPECT_EQ(copy, source);
    ASSERT_TRUE(copy.remove(Temperature(10)));
    EXPECT_TRUE(source.contains(Temperature(10)));
    EXPECT_EQ(source.size(), 2U);
}

TEST(TemperatureSetLifecycle, CopyAssignmentReplacesContents)
{
    TemperatureSet source;
    source += Temperature(10);
    TemperatureSet target;
    target += Temperature(20);

    EXPECT_EQ(&(target = source), &target);
    EXPECT_EQ(target, source);
    const TemperatureSet& same{target};
    target = same;
    EXPECT_EQ(target, source);
    EXPECT_FALSE(target.contains(Temperature(20)));
    ASSERT_TRUE(target.remove(Temperature(10)));
    EXPECT_TRUE(source.contains(Temperature(10)));
}

TEST(TemperatureSetLifecycle, MoveConstructorEmptiesSource)
{
    TemperatureSet source;
    source += Temperature(10);
    source += Temperature(20);

    TemperatureSet target(std::move(source));
    EXPECT_EQ(target.size(), 2U);
    EXPECT_TRUE(target.contains(Temperature(10)));
    EXPECT_TRUE(target.contains(Temperature(20)));
    EXPECT_TRUE(source.empty());
    EXPECT_FALSE(source.contains(Temperature(10)));
    EXPECT_TRUE(source.add(Temperature(30)));
    EXPECT_FALSE(target.contains(Temperature(30)));
}

TEST(TemperatureSetLifecycle, MoveAssignmentHandlesEmptySourceAndSelf)
{
    TemperatureSet source;
    source += Temperature(10);
    TemperatureSet target;
    target += Temperature(20);

    EXPECT_EQ(&(target = std::move(source)), &target);
    EXPECT_TRUE(source.empty());
    EXPECT_TRUE(target.contains(Temperature(10)));
    EXPECT_FALSE(target.contains(Temperature(20)));

    TemperatureSet& same{target};
    target = std::move(same);
    EXPECT_TRUE(target.contains(Temperature(10)));
    target = std::move(source);
    EXPECT_TRUE(target.empty());
    EXPECT_TRUE(target.add(Temperature(30)));
}
