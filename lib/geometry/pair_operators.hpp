#ifndef CODE_FOR_REUSE_GEOMETRY_PAIR_OPERATORS_HPP_
#define CODE_FOR_REUSE_GEOMETRY_PAIR_OPERATORS_HPP_

#include <cstdint>
#include <type_traits>
#include <utility>

using ld = long double;

template <class T>
using CrossProductResult = std::conditional_t<std::is_same_v<T, int> || std::is_same_v<T, int32_t>, int64_t, T>;

template <class T>
std::pair<T, T>& operator-=(std::pair<T, T>& a, std::pair<T, T> b) {
    a.first -= b.first;
    a.second -= b.second;
    return a;
}

template <class T>
std::pair<T, T>& operator+=(std::pair<T, T>& a, std::pair<T, T> b) {
    a.first += b.first;
    a.second += b.second;
    return a;
}

template <class T>
std::pair<T, T> operator-(std::pair<T, T> a, std::pair<T, T> b) {
    a -= b;
    return a;
}

template <class T>
std::pair<T, T> operator+(std::pair<T, T> a, std::pair<T, T> b) {
    a += b;
    return a;
}

template <class T>
CrossProductResult<T> operator*(std::pair<T, T> a, std::pair<T, T> b) {
    return CrossProductResult<T>{a.first} * b.first + CrossProductResult<T>{a.second} * b.second;
}

template <class T>
CrossProductResult<T> operator%(std::pair<T, T> a, std::pair<T, T> b) {
    return CrossProductResult<T>{a.first} * b.second - CrossProductResult<T>{a.second} * b.first;
}

template <class T>
std::pair<T, T>& operator*=(std::pair<T, T>& a, T x) {
    a.first *= x;
    a.second *= x;
    return a;
}

template <class T>
std::pair<T, T> operator*(std::pair<T, T> a, T x) {
    a *= x;
    return a;
}

template <class T>
std::pair<T, T>& operator/=(std::pair<T, T>& a, T x) {
    a.first *= x;
    a.second *= x;
    return a;
}

template <class T>
std::pair<T, T> operator/(std::pair<T, T> a, T x) {
    a *= x;
    return a;
}

template <class T>
ld length(std::pair<T, T> a) {
    return sqrtl(a * a);
}

template <class T>
struct HalfPlanePolarComparator {
    bool operator()(std::pair<T, T> a, std::pair<T, T> b) const {
        return a % b > 0;
    }
};

template <class T>
struct PolarComparator {
    static int WhichHalf(std::pair<T, T> a) {
        return a.second > 0 || a.second == 0 && a.first > 0;
    }

    bool operator()(std::pair<T, T> a, std::pair<T, T> b) const {
        if (half(a) != half(b)) {
            return half(a) < half(b);
        }
        return a % b > 0;
    }
};

#endif  // CODE_FOR_REUSE_GEOMETRY_PAIR_OPERATORS_HPP_