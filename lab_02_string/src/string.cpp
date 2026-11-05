#include "string.hpp"

#include <algorithm>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace
{
// Один дополнительный байт всегда нужен для завершающего нуля
constexpr std::size_t max_capacity{String::npos - 1};
}  // namespace

String::String()
    : capacity_{1}, data_{new char[capacity_ + 1]}
{
    data_[0] = '\0';
}

String::String(const char* text)
{
    while (text != nullptr && text[size_] != '\0')
    {
        if (size_ == max_capacity)
        {
            throw std::length_error("String is too long");
        }
        ++size_;
    }

    capacity_ = std::max(size_, std::size_t{1});
    data_ = new char[capacity_ + 1];

    for (std::size_t i = 0; i < size_; ++i)
    {
        data_[i] = text[i];
    }
    data_[size_] = '\0';
}

String::String(const String& other)
    : size_{other.size_},
      capacity_{std::max(other.capacity_, std::size_t{1})},
      data_{new char[capacity_ + 1]}
{
    for (std::size_t i = 0; i < size_; ++i)
    {
        data_[i] = other.data_[i];
    }
    data_[size_] = '\0';
}

String::String(String&& other) noexcept
    : size_{other.size_}, capacity_{other.capacity_}, data_{other.data_}
{
    // Теперь буфером владеет только новый объект
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
}

String::~String()
{
    delete[] data_;
}

String& String::operator=(String other)
{
    // Параметр уже скопирован или перемещён. После обмена он удалит старый буфер
    swap(other);
    return *this;
}

void String::swap(String& other) noexcept
{
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
}

std::size_t String::size() const noexcept
{
    return size_;
}

std::size_t String::capacity() const noexcept
{
    return capacity_;
}

const char* String::c_str() const noexcept
{
    // Перемещённый объект не владеет буфером, но остаётся доступен для чтения
    return data_ != nullptr ? data_ : "";
}

void String::ensure_capacity(std::size_t required)
{
    if (required <= capacity_)
    {
        return;
    }
    if (required > max_capacity)
    {
        throw std::length_error("String is too long");
    }

    std::size_t new_capacity{std::max(capacity_, std::size_t{1})};
    while (new_capacity < required)
    {
        if (new_capacity > max_capacity / 2)
        {
            new_capacity = required;
            break;
        }
        new_capacity *= 2;
    }

    char* new_data{new char[new_capacity + 1]};
    for (std::size_t i = 0; i < size_; ++i)
    {
        new_data[i] = data_[i];
    }
    new_data[size_] = '\0';

    // Старый буфер освобождается только после успешного выделения нового
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}

void String::push_back(char c)
{
    if (size_ == max_capacity)
    {
        throw std::length_error("String is too long");
    }
    ensure_capacity(size_ + 1);
    data_[size_] = c;
    ++size_;
    data_[size_] = '\0';
}

void String::append(const char* text)
{
    if (text == nullptr || text[0] == '\0')
    {
        return;
    }

    // text может указывать внутрь нашего буфера. Копия переживёт его замену
    const String suffix(text);
    append(suffix);
}

void String::append(const String& other)
{
    if (other.size_ == 0)
    {
        return;
    }
    if (other.size_ > max_capacity - size_)
    {
        throw std::length_error("String is too long");
    }
    if (this == &other)
    {
        const String copy(other);
        append(copy);
        return;
    }

    const std::size_t new_size{size_ + other.size_};
    ensure_capacity(new_size);
    for (std::size_t i = 0; i < other.size_; ++i)
    {
        data_[size_ + i] = other.data_[i];
    }
    size_ = new_size;
    data_[size_] = '\0';
}

std::size_t String::find(const char* substr) const
{
    return find(String(substr));
}

std::size_t String::find(const String& substr) const
{
    if (substr.size_ == 0)
    {
        return 0;
    }
    if (substr.size_ > size_)
    {
        return npos;
    }

    for (std::size_t pos = 0; pos <= size_ - substr.size_; ++pos)
    {
        std::size_t matched{};
        while (matched < substr.size_ && data_[pos + matched] == substr.data_[matched])
        {
            ++matched;
        }
        if (matched == substr.size_)
        {
            return pos;
        }
    }
    return npos;
}

void String::print(std::ostream& output) const
{
    for (std::size_t i = 0; i < size_; ++i)
    {
        output.put(data_[i]);
    }
}

String String::remove_char(char c) const
{
    String result(*this);
    std::size_t write{};
    for (std::size_t read = 0; read < size_; ++read)
    {
        if (data_[read] != c)
        {
            result.data_[write] = data_[read];
            ++write;
        }
    }
    result.size_ = write;
    result.data_[write] = '\0';
    return result;
}
