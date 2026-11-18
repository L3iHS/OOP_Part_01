#include "temperature.hpp"

#include <cmath>

Temperature::Temperature(double value, Unit unit) noexcept : celsius_{value}
{
    switch (unit)
    {
    case Unit::Celsius:
        break;
    case Unit::Fahrenheit:
        celsius_ = (value - 32.0) / 1.8;
        break;
    case Unit::Kelvin:
        celsius_ = value - 273.15;
        break;
    }

    if (is_valid())
    {
        // округляем дробную часть, чтобы погрешность пересчёта не создавала дубликаты
        constexpr double scale{1'000'000.0};
        double whole{};
        const double fraction{std::modf(celsius_, &whole)};
        celsius_ = whole + std::round(fraction * scale) / scale;
        if (celsius_ == 0.0)
        {
            celsius_ = 0.0;
        }
    }
}

bool Temperature::is_valid() const noexcept
{
    return std::isfinite(celsius_);
}

bool Temperature::operator==(const Temperature& other) const noexcept
{
    return celsius_ == other.celsius_;
}

std::ostream& operator<<(std::ostream& out, const Temperature& temperature)
{
    return out << temperature.celsius_ << " °C";
}
