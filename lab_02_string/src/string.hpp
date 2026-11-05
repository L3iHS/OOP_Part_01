#pragma once

#include <cstddef>
#include <iosfwd>
#include <limits>

class String
{
    std::size_t size_{};
    std::size_t capacity_{};
    char* data_{nullptr};

    void ensure_capacity(std::size_t required);

public:
    static constexpr std::size_t npos{std::numeric_limits<std::size_t>::max()};

    String();
    explicit String(const char* text);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    String& operator=(String other);
    void swap(String& other) noexcept;

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    const char* c_str() const noexcept;

    void push_back(char c);
    void append(const char* text);
    void append(const String& other);
    std::size_t find(const char* substr) const;
    std::size_t find(const String& substr) const;
    void print(std::ostream& output) const;

    String remove_char(char c) const;
};
