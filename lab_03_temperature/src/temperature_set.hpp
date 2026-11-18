#pragma once

#include "temperature.hpp"

#include <cstddef>

class TemperatureSet
{
    Temperature* data_{nullptr};
    std::size_t size_{0};
    std::size_t capacity_{0};

    void swap(TemperatureSet& other) noexcept;
    std::size_t find(const Temperature& temperature) const noexcept;
    void append(const Temperature& temperature);

public:
    TemperatureSet() = default;
    TemperatureSet(const TemperatureSet& other);
    TemperatureSet(TemperatureSet&& other) noexcept;
    TemperatureSet& operator=(const TemperatureSet& other);
    TemperatureSet& operator=(TemperatureSet&& other) noexcept;
    ~TemperatureSet();

    bool add(const Temperature& temperature);
    bool remove(const Temperature& temperature) noexcept;
    bool contains(const Temperature& temperature) const noexcept;
    bool operator()(const Temperature& temperature) const noexcept;

    std::size_t size() const noexcept;
    bool empty() const noexcept;

    TemperatureSet& operator+=(const Temperature& temperature);
    TemperatureSet& operator-=(const Temperature& temperature) noexcept;

    TemperatureSet operator|(const TemperatureSet& other) const;
    TemperatureSet operator&(const TemperatureSet& other) const;
    TemperatureSet operator-(const TemperatureSet& other) const;

    bool operator==(const TemperatureSet& other) const noexcept;
    bool operator!=(const TemperatureSet& other) const noexcept;

    friend std::ostream& operator<<(std::ostream& out, const TemperatureSet& set);
};
