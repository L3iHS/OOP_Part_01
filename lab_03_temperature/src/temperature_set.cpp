#include "temperature_set.hpp"

#include <utility>

TemperatureSet::TemperatureSet(const TemperatureSet& other)
    : data_{other.capacity_ == 0 ? nullptr : new Temperature[other.capacity_]},
      size_{other.size_}, capacity_{other.capacity_}
{
    for (std::size_t i = 0; i < size_; ++i)
    {
        data_[i] = other.data_[i];
    }
}

TemperatureSet::TemperatureSet(TemperatureSet&& other) noexcept
{
    swap(other);
}

TemperatureSet& TemperatureSet::operator=(const TemperatureSet& other)
{
    if (this != &other)
    {
        TemperatureSet copy(other);
        swap(copy);
    }
    return *this;
}

TemperatureSet& TemperatureSet::operator=(TemperatureSet&& other) noexcept
{
    if (this != &other)
    {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        swap(other);
    }
    return *this;
}

TemperatureSet::~TemperatureSet()
{
    delete[] data_;
}

void TemperatureSet::swap(TemperatureSet& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

std::size_t TemperatureSet::find(const Temperature& temperature) const noexcept
{
    std::size_t i{0};
    while (i < size_ && !(data_[i] == temperature))
    {
        ++i;
    }
    return i;
}

void TemperatureSet::append(const Temperature& temperature)
{
    if (size_ == capacity_)
    {
        const std::size_t new_capacity{capacity_ == 0 ? 1 : capacity_ * 2};
        Temperature* new_data{new Temperature[new_capacity]};
        for (std::size_t i = 0; i < size_; ++i)
        {
            new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    data_[size_] = temperature;
    ++size_;
}

bool TemperatureSet::add(const Temperature& temperature)
{
    if (!temperature.is_valid() || contains(temperature))
    {
        return false;
    }

    append(temperature);
    return true;
}

bool TemperatureSet::remove(const Temperature& temperature) noexcept
{
    const std::size_t index{find(temperature)};
    if (index == size_)
    {
        return false;
    }

    for (std::size_t i = index + 1; i < size_; ++i)
    {
        data_[i - 1] = data_[i];
    }
    --size_;
    return true;
}

bool TemperatureSet::contains(const Temperature& temperature) const noexcept
{
    return find(temperature) != size_;
}

bool TemperatureSet::operator()(const Temperature& temperature) const noexcept
{
    return contains(temperature);
}

std::size_t TemperatureSet::size() const noexcept
{
    return size_;
}

bool TemperatureSet::empty() const noexcept
{
    return size_ == 0;
}

TemperatureSet& TemperatureSet::operator+=(const Temperature& temperature)
{
    add(temperature);
    return *this;
}

TemperatureSet& TemperatureSet::operator-=(const Temperature& temperature) noexcept
{
    remove(temperature);
    return *this;
}

TemperatureSet TemperatureSet::operator|(const TemperatureSet& other) const
{
    TemperatureSet result(*this);
    for (std::size_t i = 0; i < other.size_; ++i)
    {
        result.add(other.data_[i]);
    }
    return result;
}

TemperatureSet TemperatureSet::operator&(const TemperatureSet& other) const
{
    TemperatureSet result;
    for (std::size_t i = 0; i < size_; ++i)
    {
        if (other.contains(data_[i]))
        {
            result.append(data_[i]);
        }
    }
    return result;
}

TemperatureSet TemperatureSet::operator-(const TemperatureSet& other) const
{
    TemperatureSet result;
    for (std::size_t i = 0; i < size_; ++i)
    {
        if (!other.contains(data_[i]))
        {
            result.append(data_[i]);
        }
    }
    return result;
}

bool TemperatureSet::operator==(const TemperatureSet& other) const noexcept
{
    if (size_ != other.size_)
    {
        return false;
    }

    for (std::size_t i = 0; i < size_; ++i)
    {
        if (!other.contains(data_[i]))
        {
            return false;
        }
    }
    return true;
}

bool TemperatureSet::operator!=(const TemperatureSet& other) const noexcept
{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const TemperatureSet& set)
{
    out << '{';
    for (std::size_t i = 0; i < set.size_; ++i)
    {
        out << (i == 0 ? " " : ", ") << set.data_[i];
    }
    if (!set.empty())
    {
        out << ' ';
    }
    return out << '}';
}
