#pragma once

#include <iostream>

class Temperature
{
    double celsius_{0.0};

public:
    enum class Unit
    {
        Celsius,
        Fahrenheit,
        Kelvin
    };

    Temperature() = default;
    explicit Temperature(double value, Unit unit = Unit::Celsius) noexcept;

    bool is_valid() const noexcept;
    bool operator==(const Temperature& other) const noexcept;

    friend std::ostream& operator<<(std::ostream& out, const Temperature& temperature);
};
