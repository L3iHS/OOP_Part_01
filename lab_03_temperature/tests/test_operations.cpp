#include "temperature_set.hpp"

#include <gtest/gtest.h>
#include <limits>
#include <sstream>

using Unit = Temperature::Unit;

TEST(TemperatureSetOperations, EmptySet)
{
    const TemperatureSet set;
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0U);
    EXPECT_FALSE(set.contains(Temperature(0)));
    EXPECT_FALSE(set(Temperature(0)));
    std::ostringstream out;
    out << set;
    EXPECT_EQ(out.str(), "{}");
}

TEST(TemperatureSetOperations, AddChecksCanonicalValue)
{
    TemperatureSet set;
    const Temperature celsius(20);
    const Temperature fahrenheit(68, Unit::Fahrenheit);
    ASSERT_TRUE(set.add(celsius));
    EXPECT_FALSE(set.add(fahrenheit));
    EXPECT_TRUE(set.contains(fahrenheit));
    EXPECT_TRUE(set(fahrenheit));
    EXPECT_EQ(set.size(), 1U);
}

TEST(TemperatureSetOperations, RejectsNonFiniteValues)
{
    TemperatureSet set;
    const Temperature nan(std::numeric_limits<double>::quiet_NaN());
    const Temperature infinity(std::numeric_limits<double>::infinity(), Unit::Kelvin);
    EXPECT_FALSE(set.add(nan));
    EXPECT_FALSE(set.add(infinity));
    EXPECT_TRUE(set.empty());
}

TEST(TemperatureSetOperations, RemovePreservesOrder)
{
    TemperatureSet set;
    set += Temperature(30);
    set += Temperature(20);
    set += Temperature(10);
    set += Temperature(40);

    EXPECT_TRUE(set.remove(Temperature(293.15, Unit::Kelvin)));
    EXPECT_FALSE(set.remove(Temperature(20)));
    EXPECT_FALSE(set.contains(Temperature(20)));
    EXPECT_FALSE(set(Temperature(20)));
    EXPECT_EQ(set.size(), 3U);

    std::ostringstream out;
    out << set;
    EXPECT_EQ(out.str(), "{ 30 °C, 10 °C, 40 °C }");
}

TEST(TemperatureSetOperations, CompoundOperatorsReturnSet)
{
    TemperatureSet set;
    EXPECT_EQ(&((set += Temperature(0)) += Temperature(10)), &set);
    EXPECT_EQ(&((set -= Temperature(10)) -= Temperature(0)), &set);
    EXPECT_TRUE(set.empty());
}

TEST(TemperatureSetOperations, EqualityIgnoresInsertionOrder)
{
    TemperatureSet first;
    first += Temperature(10);
    first += Temperature(0);

    TemperatureSet second;
    second += Temperature(32, Unit::Fahrenheit);
    second += Temperature(283.15, Unit::Kelvin);

    EXPECT_TRUE(first == second);
    EXPECT_FALSE(first != second);
    second -= Temperature(0);
    second += Temperature(20);
    EXPECT_FALSE(first == second);
    EXPECT_TRUE(first != second);
}

TEST(TemperatureSetOperations, ComputesSetOperations)
{
    TemperatureSet first;
    first += Temperature(20);
    first += Temperature(0);
    first += Temperature(30);

    TemperatureSet second;
    second += Temperature(68, Unit::Fahrenheit);
    second += Temperature(10);

    std::ostringstream out;
    out << (first | second) << '\n' << (first & second) << '\n' << (first - second);
    EXPECT_EQ(out.str(), "{ 20 °C, 0 °C, 30 °C, 10 °C }\n"
                         "{ 20 °C }\n{ 0 °C, 30 °C }");
    EXPECT_EQ(first.size(), 3U);
    EXPECT_EQ(second.size(), 2U);
    EXPECT_EQ(first | first, first);
    EXPECT_EQ(first & first, first);
    EXPECT_TRUE((first - first).empty());
}

TEST(TemperatureSetOperations, AddsThousandUniqueTemperatures)
{
    TemperatureSet set;
    for (int i = 0; i < 1000; ++i)
    {
        ASSERT_TRUE(set.add(Temperature(i)));
    }
    for (int i = 0; i < 1000; ++i)
    {
        const Temperature equivalent(273.15 + i, Unit::Kelvin);
        EXPECT_TRUE(set.contains(equivalent));
        EXPECT_FALSE(set.add(equivalent));
    }
    EXPECT_EQ(set.size(), 1000U);
}
