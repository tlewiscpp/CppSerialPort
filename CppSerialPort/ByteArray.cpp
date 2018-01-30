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
    return *this;
}

ByteArray &ByteArray::operator=(const std::vector<char> &rhs) {
    this->m_buffer.clear();
    std::copy(rhs.begin(), rhs.end(), std::back_inserter(this->m_buffer));
    return *this;
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

ByteArray &ByteArray::operator+(char c) {
    this->append(c);
    return *this;
}

ByteArray &ByteArray::operator+(const ByteArray &rhs) {
    this->append(rhs);
    return *this;
}

ByteArray &ByteArray::operator+(const std::string &rhs) {
    this->append(rhs);
    return *this;
}

ByteArray &ByteArray::operator+(const std::vector<char> &rhs) {
    this->append(rhs);
    return *this;
}

ByteArray operator+(char c, const ByteArray &rhs) {
    ByteArray returnArray{c};
    returnArray += rhs;
    return returnArray;
}

ByteArray operator+(const ByteArray &lhs, const ByteArray &rhs) {
    ByteArray returnArray{lhs};
    returnArray += rhs;
    return returnArray;
}

ByteArray operator+(const std::string &lhs, const ByteArray &rhs) {
    ByteArray returnArray{lhs};
    returnArray += rhs;
    return returnArray;
}

ByteArray operator+(const std::vector<char> &lhs, const ByteArray &rhs) {
    ByteArray returnArray{lhs};
    returnArray += rhs;
    return returnArray;
}

ByteArray operator+(const ByteArray &lhs, char c) {
    ByteArray returnArray{lhs};
    returnArray.append(c);
    return returnArray;
}

ByteArray operator+(const ByteArray &lhs, const std::string &rhs) {
    ByteArray returnArray{lhs};
    returnArray.append(rhs);
    return returnArray;
}

ByteArray operator+(const ByteArray &lhs, const std::vector<char> &rhs) {
    ByteArray returnArray{lhs};
    returnArray.append(rhs);
    return returnArray;
}

const std::vector<char>::const_iterator ByteArray::cbegin() const {
    return this->m_buffer.cbegin();
}

std::vector<char>::iterator ByteArray::begin() {
    return this->m_buffer.begin();
}

std::vector<char>::reverse_iterator ByteArray::rbegin() {
    return this->m_buffer.rbegin();
}

std::vector<char>::const_reverse_iterator ByteArray::crbegin() const {
    return this->m_buffer.crbegin();
}

std::vector<char>::iterator ByteArray::end() {
    return this->m_buffer.end();
}

const std::vector<char>::const_iterator ByteArray::cend() const {
    return this->m_buffer.cend();
}

std::vector<char>::reverse_iterator ByteArray::rend() {
    return this->m_buffer.rend();
}

std::vector<char>::const_reverse_iterator ByteArray::crend() const {
    return this->m_buffer.crend();
}

}