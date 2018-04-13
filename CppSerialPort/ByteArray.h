#ifndef CPPSERIALPORT_BYTEARRAY_H
#define CPPSERIALPORT_BYTEARRAY_H

#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace Detail {
    template <template<typename> class Trait, typename First, typename ...Args>
    struct is_all_same_type {
        enum { value = Trait<First>::value && is_all_same_type<Trait, Args...>::value };
    };

    template <template<typename> class Trait, typename Head>
    struct is_all_same_type<Trait, Head> {
        enum { value = Trait<Head>::value };
    };

    template <typename T>
    using UIntRequirement = std::is_same<unsigned int, T>;

    template <typename T>
    using IntRequirement = std::is_same<int, T>;

    template <typename T>
    using CharRequirement = std::is_same<char, T>;

    template <typename T>
    using UCharRequirement = std::is_same<unsigned char, T>;

    template <typename T>
    using ShortRequirement = std::is_same<short, T>;

    template <typename T>
    using UShortRequirement = std::is_same<unsigned short, T>;
}

namespace CppSerialPort {

class ByteArray {
public:
    explicit ByteArray() = default;
    explicit ByteArray(const char *cStr);
    explicit ByteArray(const std::string &str);
    explicit ByteArray(char *buffer, size_t length);
    explicit ByteArray(char *buffer, int length);
    ByteArray &operator=(const ByteArray &rhs);
    ByteArray &operator=(const std::vector<char> &rhs);
    ByteArray &operator=(const std::string &rhs);
    ByteArray &operator=(char c);
    ByteArray &operator=(ByteArray &&rhs) noexcept;
    ByteArray &operator=(std::vector<char> &&rhs);
    ByteArray(const ByteArray &other) = default;
    ByteArray(ByteArray &&other) noexcept = default;
    template <typename T> explicit ByteArray(const std::vector<T> &byteArray) : m_buffer{} {
        for (const auto &it : byteArray) { this->m_buffer.push_back(static_cast<char>(it)); }
    }
    template <typename ...Ts> explicit ByteArray(Ts...ts) : 
        ByteArray{std::vector<std::common_type_t<Ts...>>{ts...}} {
            static_assert(
                    Detail::is_all_same_type<Detail::UCharRequirement, Ts...>::value ||
                    Detail::is_all_same_type<Detail::CharRequirement, Ts...>::value ||
                    Detail::is_all_same_type<Detail::UShortRequirement, Ts...>::value ||
                    Detail::is_all_same_type<Detail::ShortRequirement, Ts...>::value ||
                    Detail::is_all_same_type<Detail::IntRequirement, Ts...>::value ||
                    Detail::is_all_same_type<Detail::UIntRequirement, Ts...>::value,
                    "All arguments must be of homogeneous type");
            //static_assert(is_all_same<int, Ts...>::value || is_all_same<char, Ts...>, "Arguments must be int.");
        }

    ByteArray &append(char c);
    ByteArray &append(int i);
    ByteArray &append(const ByteArray &rhs);
    ByteArray &append(const std::string &rhs);
    ByteArray &append(const std::vector<char> &rhs);
    ByteArray &operator+=(char c);
    ByteArray &operator+=(int i);
    ByteArray &operator+=(const ByteArray &rhs);
    ByteArray &operator+=(const std::string &rhs);
    ByteArray &operator+=(const std::vector<char> &rhs);
    ByteArray &operator+(int i);
    ByteArray &operator+(char c);
    ByteArray &operator+(const ByteArray &rhs);
    ByteArray &operator+(const std::string &rhs);
    ByteArray &operator+(const std::vector<char> &rhs);

    size_t find(const ByteArray &toFind);
    size_t find(char c);

    friend ByteArray operator+(char c, const ByteArray &rhs);
    friend ByteArray operator+(int i, const ByteArray &rhs);
    friend ByteArray operator+(const ByteArray &lhs, const ByteArray &rhs);
    friend ByteArray operator+(const std::string &lhs, const ByteArray &rhs);
    friend ByteArray operator+(const std::vector<char> &lhs, const ByteArray &rhs);
    friend ByteArray operator+(const ByteArray &lhs, char c);
    friend ByteArray operator+(const ByteArray &lhs, int c);
    friend ByteArray operator+(const ByteArray &lhs, const std::string &rhs);
    friend ByteArray operator+(const ByteArray &lhs, const std::vector<char> &rhs);

    std::vector<char>::iterator begin();
    const std::vector<char>::const_iterator cbegin() const;
    std::vector<char>::reverse_iterator rbegin();
    const std::vector<char>::const_reverse_iterator crbegin() const;
    std::vector<char>::iterator end();
    const std::vector<char>::const_iterator cend() const;
    std::vector<char>::reverse_iterator rend();
    const std::vector<char>::const_reverse_iterator crend() const;

    ByteArray &clear();
    size_t size() const;
    size_t length() const;
    bool empty() const;
    const char *data() const;
    char *data();

    ByteArray &popBack();
    ByteArray &popFront();

    char &operator[](size_t index);
    const char &operator[](size_t index) const;
    char &at(size_t index);
    const char &at(size_t index) const;

    ByteArray subsequence(size_t index, size_t length = 0) const;

    friend bool operator==(const ByteArray &lhs, const ByteArray &rhs) { return std::equal(lhs.m_buffer.begin(), lhs.m_buffer.end(), rhs.m_buffer.begin()); }
    explicit operator std::string() const;
    std::string toString() const;
    std::string prettyPrint(int spacing) const;
    std::string prettyPrint() const;

    bool endsWith(char *buffer, size_t length) const;
    bool endsWith(const char *cStr) const;
    bool endsWith(const std::string &str) const;
    bool endsWith(const ByteArray &byteArray) const;

    bool startsWith(const ByteArray &byteArray) const;
    bool startsWith(char *buffer, size_t length) const;
    bool startsWith(const char *cStr) const;
    bool startsWith(const std::string &str) const;
private:
    std::vector<char> m_buffer;

    bool endsWith(const std::vector<char> &ending) const;
    bool startsWith(const std::vector<char> &start) const;
};

} //namespace CppSerialPort


#endif //CPPSERIALPORT_BYTEARRAY_H
