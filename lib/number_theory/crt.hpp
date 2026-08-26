#ifndef CODE_FOR_REUSE_NUMBER_THEORY_CRT_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_CRT_HPP_

#include <cstdint>
#include <memory>

#include "exgcd.hpp"

int64_t crt(int64_t a, int64_t m, int64_t b, int64_t n) {
    if (n > m) std::swap(a, b), std::swap(m, n);
    int64_t x, y, g = exgcd(m, n, x, y);
    x = (b - a) % n * x % n / g * m + a;
    return x < 0 ? x + m * n / g : x;
}

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_CRT_HPP_