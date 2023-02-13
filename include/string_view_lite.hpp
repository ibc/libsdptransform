#pragma once

#include <string>
#include <algorithm>
#include <ostream>

namespace sdptransform
{
    // lite version of string_view for c++14
    class string_view_lite
    {
    public:
        // types
        using Traits_type = std::char_traits<char>;
        using value_type = char;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using const_iterator = const value_type *;
        using iterator = const_iterator;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator = const_reverse_iterator;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        static constexpr size_type npos = size_type(-1);

        constexpr string_view_lite() noexcept {}
        constexpr string_view_lite(const string_view_lite &) noexcept = default;
        constexpr string_view_lite &operator=(const string_view_lite &) noexcept = default;
        string_view_lite(const value_type *str) : data_(str), size_(Traits_type::length(str)) {}
        constexpr string_view_lite(std::nullptr_t) = delete;
        string_view_lite(const value_type *str, size_type len) : data_(str), size_(len)
        {
            if (Traits_type::length(str) < size_)
            {
                throw std::runtime_error("string_view_lite constructor : wrong length");
            }
        }
        string_view_lite(std::string::const_iterator begin, std::string::const_iterator end) : data_(&(*begin)), size_(end - begin) {}
        string_view_lite(const std::string &s) : data_(s.data()), size_(s.size()) {}

        // iterator support
        constexpr const_iterator begin() const noexcept { return data_; }
        constexpr const_iterator end() const noexcept { return data_ + size_; }
        constexpr const_iterator cbegin() const noexcept { return begin(); }
        constexpr const_iterator cend() const noexcept { return end(); }
        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        const_reverse_iterator crbegin() const noexcept { return rbegin(); }
        const_reverse_iterator crend() const noexcept { return rend(); }

        // capacity
        constexpr size_type size() const noexcept { return size_; }
        constexpr size_type length() const noexcept { return size(); }
        constexpr size_type max_size() const noexcept { return npos; }
        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return size() == 0U;
        }

        // element access
        constexpr const_reference operator[](size_type pos) const { return *(data_ + pos); }
        constexpr const_reference at(size_type pos) const
        {
            if (pos >= size_)
            {
                throw std::out_of_range("string_view_lite at");
            }
            return operator[](pos);
        }
        constexpr const_reference front() const
        {
            if (empty())
            {
                throw std::out_of_range("string_view_lite front");
            }
            return *begin();
        }
        constexpr const_reference back() const
        {
            if (empty())
            {
                throw std::out_of_range("string_view_lite back");
            }
            return *(end() - 1);
        }
        constexpr const_pointer data() const noexcept { return data_; }

        // modifiers
        constexpr void remove_prefix(size_type n)
        {
            if (n >= size_)
            {
                string_view_lite sv;
                swap(sv);
            }
            else
            {
                data_ += n;
                size_ -= n;
            }
        }
        constexpr void remove_suffix(size_type n)
        {
            if (n >= size_)
            {
                string_view_lite sv;
                swap(sv);
            }
            else
            {
                size_ -= n;
            }
        }
        void swap(string_view_lite &s) noexcept
        {
            std::swap(data_, s.data_);
            std::swap(size_, s.size_);
        }

        // string operations

        string_view_lite substr(size_type pos = 0, size_type n = npos) const
        {
            if (pos > size_)
            {
                throw std::out_of_range("string_view_lite substr");
            }
            if (pos == size_)
            {
                return string_view_lite();
            }
            if (n == npos || size_ < (n + pos))
            {
                return string_view_lite(data_ + pos, size_ - pos);
            }
            return string_view_lite(data_ + pos, n);
        }

        // searching
        constexpr size_type find(string_view_lite s, size_type pos = 0) const noexcept
        {
            return pos >= size() ? npos : std::search(cbegin() + pos, cend(), s.cbegin(), s.cend(), Traits_type::eq) - cbegin();
        }
        constexpr size_type find(value_type c, size_type pos = 0) const noexcept
        {
            return (empty() || pos >= size()) ? npos : std::find(cbegin() + pos, cend(), c) - cbegin();
        }

        constexpr size_type find_first_of(value_type c, size_type pos = 0) const noexcept
        {
            return find(c, pos);
        }
        size_type find_last_of(value_type c, size_type pos = npos) const noexcept
        {
            return empty() ? npos : std::find(const_reverse_iterator(cbegin() + pos + 1), crend(), c) - crbegin();
        }
        constexpr size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept
        {
            return (pos >= size() || empty())
                       ? npos
                       : std::find_if(cbegin() + pos, cend(), [&c](char other)
                                      { return other != c; }) -
                             cbegin();
        }
        constexpr size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept
        {
            return empty() ? npos : std::find_if(const_reverse_iterator(cbegin() + pos + 1), crend(), [&c](char other)
                                                 { return other != c; }) -
                                        crbegin();
        }

    private:
        const_pointer data_ = ""; // exposition only
        size_type size_ = 0U;     // exposition only
    };

    std::basic_ostream<string_view_lite::value_type, string_view_lite::Traits_type> &
    operator<<(std::basic_ostream<string_view_lite::value_type, string_view_lite::Traits_type> &os,
               string_view_lite str)
    {
        for (char c : str)
        {
            os << c;
        }
        return os;
    }

} // namespace sdptransform
