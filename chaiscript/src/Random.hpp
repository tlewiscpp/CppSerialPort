#ifndef CPPSERIALPORT_RANDOM_HPP
#define CPPSERIALPORT_RANDOM_HPP

#include <random>

class Random
{
public:
    Random() :
        m_randomEngine{std::random_device{}()}
    {

    }

    Random(const Random &rhs) = default;
    Random(Random &&rhs) noexcept = default;
    Random &operator=(const Random &rhs) = default;
    Random &operator=(Random &&rhs) noexcept = default;

    template <typename T> T randomBetween(T lowLimit, T highLimit, bool lowInclusive, bool highInclusive) {
        int low{lowInclusive ? lowLimit : lowLimit + 1};
        int high{highInclusive ? highLimit : highLimit - 1};
        return std::uniform_int_distribution<T>{low, high}(this->m_randomEngine);
    }

    template <typename T> T randomBetween(T lowLimit, T highLimit) {
        return randomBetween<T>(lowLimit, highLimit, false, false);
    }
private:
    std::mt19937 m_randomEngine;
};

#endif //CPPSERIALPORT_RANDOM_HPP
