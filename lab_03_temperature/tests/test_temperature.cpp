#include "temperature.hpp"

#include <gtest/gtest.h>
#include <sstream>

using Unit = Temperature::Unit;

TEST(TemperatureConstruction, DefaultIsZeroCelsius)
{
    const Temperature value;
    EXPECT_TRUE(value.is_valid());
    EXPECT_EQ(value, Temperature(0));
    EXPECT_EQ(value, Temperature(32, Unit::Fahrenheit));
}

TEST(TemperatureComparison, NormalizesToCelsius)
{
    EXPECT_EQ(Temperature(20), Temperature(68, Unit::Fahrenheit));
    EXPECT_EQ(Temperature(20), Temperature(293.15, Unit::Kelvin));
    EXPECT_EQ(Temperature(-40), Temperature(-40, Unit::Fahrenheit));
    EXPECT_EQ(Temperature(20), Temperature(20.0000004));
    EXPECT_NE(Temperature(20), Temperature(20.0000006));
}

TEST(TemperatureOutput, UsesCanonicalUnit)
{
    std::ostringstream out;
    out << Temperature(69.8, Unit::Fahrenheit) << ", " << Temperature(-0.0000004);
    EXPECT_EQ(out.str(), "21 °C, 0 °C");
}
