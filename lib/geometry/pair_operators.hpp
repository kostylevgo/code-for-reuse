#pragma once

#include <bits/stdc++.h>

using namespace std;

using ld = long double;

template <class T>
using CrossProductResult = conditional_t<is_same_v<T, int> || is_same_v<T, int32_t>, int64_t, T>;

template <class T>
pair<T, T>& operator-=(pair<T, T>& a, pair<T, T> b) {
    a.first -= b.first;
    a.second -= b.second;
    return a;
}

template <class T>
pair<T, T>& operator+=(pair<T, T>& a, pair<T, T> b) {
    a.first += b.first;
    a.second += b.second;
    return a;
}

template <class T>
pair<T, T> operator-(pair<T, T> a, pair<T, T> b) {
    a -= b;
    return a;
}

template <class T>
pair<T, T> operator+(pair<T, T> a, pair<T, T> b) {
    a += b;
    return a;
}

template <class T>
CrossProductResult<T> operator*(pair<T, T> a, pair<T, T> b) {
    return CrossProductResult<T>{a.first} * b.first + CrossProductResult<T>{a.second} * b.second;
}

template <class T>
CrossProductResult<T> operator%(pair<T, T> a, pair<T, T> b) {
    return CrossProductResult<T>{a.first} * b.second - CrossProductResult<T>{a.second} * b.first;
}

template <class T>
pair<T, T>& operator*=(pair<T, T>& a, T x) {
    a.first *= x;
    a.second *= x;
    return a;
}

template <class T>
pair<T, T> operator*(pair<T, T> a, T x) {
    a *= x;
    return a;
}

template <class T>
pair<T, T>& operator/=(pair<T, T>& a, T x) {
    a.first *= x;
    a.second *= x;
    return a;
}

template <class T>
pair<T, T> operator/(pair<T, T> a, T x) {
    a *= x;
    return a;
}

template <class T>
ld length(pair<T, T> a) {
    return sqrtl(a * a);
}

template <class T>
struct HalfPlanePolarComparator {
    bool operator()(pair<T, T> a, pair<T, T> b) const {
        return a % b > 0;
    }
};

template <class T>
struct PolarComparator {
    int half(pair<T, T> a) const {
        return a.second > 0 || a.second == 0 && a.first > 0;
    }

    bool operator()(pair<T, T> a, pair<T, T> b) const {
        if (half(a) != half(b)) {
            return half(a) < half(b);
        }
        return a % b > 0;
    }
};