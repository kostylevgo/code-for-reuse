#ifndef CODE_FOR_REUSE_NUMBER_THEORY_EXGCD_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_EXGCD_HPP_

#include <cstdint>

int64_t exgcd(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if (!b) return x = 1, y = 0, a;
    int64_t d = exgcd(b, a % b, y, x);
    return y -= a / b * x, d;
}

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_EXGCD_HPP_