#include "ByteArray.h"

#include <cstring>
#include <stdexcept>
#include <sstream>

namespace {
    template <typename T> inline std::string toStdString(const T &t) { return dynamic_cast<std::ostringstream &>(std::ostringstream{} << t).str(); }
}

namespace CppSerialPort {

ByteArray::ByteArray(char c) :
    ByteArray{&c, 1}
{

}

ByteArray::ByteArray(const char *cStr) :
    m_buffer{cStr, cStr + strlen(cStr)}
{

}

ByteArray::ByteArray(const std::string &str) :
    m_buffer{str.begin(), str.end()}
{

}

ByteArray::ByteArray(char *buffer, size_t length) :
    m_buffer{buffer, buffer + length}
{

}

ByteArray::ByteArray(const std::vector<char> &byteArray) :
    m_buffer{byteArray.begin(), byteArray.end()}
{

}


ByteArray &ByteArray::clear()
{
    this->m_buffer.clear();
    return *this;
}

size_t ByteArray::size() const
{
    return this->m_buffer.size();
}

size_t ByteArray::length() const
{
    return this->m_buffer.size();
}

bool ByteArray::empty() const
{
    return this->m_buffer.empty();
}

char &ByteArray::operator[](size_t index) {
    return this->m_buffer.operator[](index);
}

char &ByteArray::at(size_t index) {
    return this->m_buffer.at(index);
}

const char *ByteArray::data() const {
    return this->m_buffer.data();
}

char *ByteArray::data() {
    return this->m_buffer.data();
}


ByteArray ByteArray::subsequence(size_t index, size_t length) const
{
    if (index > this->size()) {
        throw std::runtime_error(
                "CppSerialPort::ByteArray::subsequence(size_t, size_t): index cannot be greater than current size (" +
                toStdString(index) + " > " + toStdString(this->size()) + ")");
    }
    auto beginningIter = this->m_buffer.begin() + index;
    auto endingIter = beginningIter;

    size_t counter{0};
    while ( (endingIter != this->m_buffer.end()) && (counter++ != length) ) {
        endingIter++;
    }
    return ByteArray{beginningIter, endingIter};
}

ByteArray::operator std::string() const {
    return this->toString();
}

std::string ByteArray::toString() const {
    return std::string{this->m_buffer.begin(), this->m_buffer.end()};
}

bool ByteArray::endsWith(char *buffer, size_t length) const {
    return this->endsWith(ByteArray{buffer, length});
}

bool ByteArray::endsWith(const char *cStr) const {
    return this->endsWith(ByteArray{cStr});
}

bool ByteArray::endsWith(const ByteArray &byteArray) const {
    return ( this->endsWith(byteArray.m_buffer));
}

bool ByteArray::endsWith(const std::string &ending) const {
    return this->endsWith(ByteArray{ending});
}

bool ByteArray::endsWith(const std::vector<char> &ending) const {
    return ( (this->size() < ending.size()) ? false : std::equal(ending.rbegin(), ending.rend(), this->m_buffer.rbegin()) );
}

bool ByteArray::startsWith(char *buffer, size_t length) const {
    return this->startsWith(ByteArray{buffer, length});
}

bool ByteArray::startsWith(const char *cStr) const {
    return this->startsWith(ByteArray{cStr});
}

bool ByteArray::startsWith(const ByteArray &byteArray) const {
    return this->startsWith(byteArray.m_buffer);
}

bool ByteArray::startsWith(const std::string &start) const {
    return this->endsWith(ByteArray{start});
}

bool ByteArray::startsWith(const std::vector<char> &start) const {
    return ( (this->m_buffer.size() < start.size()) ? false : std::equal(start.begin(), start.end(), this->m_buffer.begin()) );
}

ByteArray &ByteArray::operator=(const ByteArray &rhs) {
    this->m_buffer.clear();
    std::copy(rhs.m_buffer.begin(), rhs.m_buffer.end(), std::back_inserter(this->m_buffer));
}

ByteArray &ByteArray::operator=(const std::vector<char> &rhs) {
    this->m_buffer.clear();
    std::copy(rhs.begin(), rhs.end(), std::back_inserter(this->m_buffer));
}

ByteArray &ByteArray::operator=(const std::string &rhs) {
    this->m_buffer.clear();
    std::copy(rhs.begin(), rhs.end(), std::back_inserter(this->m_buffer));
    return *this;
}

ByteArray &ByteArray::operator=(char c) {
    this->m_buffer.clear();
    this->m_buffer.push_back(c);
    return *this;
}

ByteArray &ByteArray::operator=(ByteArray &&rhs) {
    this->m_buffer.clear();
    this->m_buffer = std::move(rhs.m_buffer);
    return *this;
}

ByteArray &ByteArray::operator=(std::vector<char> &&rhs) {
    this->m_buffer.clear();
    this->m_buffer = std::move(rhs);
    return *this;
}

ByteArray &ByteArray::operator+=(char c) {
    return this->append(c);
}

ByteArray &ByteArray::append(char c) {
    this->m_buffer.push_back(c);
    return *this;
}
ByteArray &ByteArray::append(const ByteArray &rhs) {
    std::copy(rhs.m_buffer.begin(), rhs.m_buffer.end(), std::back_inserter(this->m_buffer));
    return *this;
}

ByteArray &ByteArray::append(const std::string &rhs) {
    std::copy(rhs.begin(), rhs.end(), std::back_inserter(this->m_buffer));
    return *this;
}

ByteArray &ByteArray::append(const std::vector<char> &rhs) {
    std::copy(rhs.begin(), rhs.end(), std::back_inserter(this->m_buffer));
    return *this;
}

ByteArray &ByteArray::operator+=(const ByteArray &rhs) {
    return this->append(rhs);
}

ByteArray &ByteArray::operator+=(const std::string &rhs) {
    return this->append(rhs);
}

ByteArray &ByteArray::operator+=(const std::vector<char> &rhs) {
    return this->append(rhs);
}

ByteArray &ByteArray::popBack() {
    this->m_buffer.pop_back();
    return *this;
}


/*
static inline bool endsWith (const std::string &fullString, const std::string &ending) {
    return ( (fullString.length() < ending.length()) ? false : std::equal(ending.rbegin(), ending.rend(), fullString.rbegin()) );
}
static inline bool endsWith(const std::string &fullString, char ending) {
    return ((fullString.length() > 0) && (fullString.back() == ending));
}
static inline bool startsWith(const std::string &fullString, const std::string &start) {
    return ((fullString.length() < start.length()) ? false : std::equal(start.begin(), start.end(), fullString.begin()));
}
static inline bool startsWith(const std::string &fullString, char start) {
    return ((fullString.length() > 0) && (fullString.front() == start));
}
 */

}