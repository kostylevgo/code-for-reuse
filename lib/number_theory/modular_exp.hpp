#ifndef CODE_FOR_REUSE_NUMBER_THEORY_MODULAR_EXP_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_MODULAR_EXP_HPP_

#include <cstdint>

constexpr int ModularExp(int x, int64_t y, int mod) {
    int c = 1;
    while (y) {
        if (y & 1) c = int64_t{c} * x % mod;
        y >>= 1;
        x = int64_t{x} * x % mod;
    }
    return c;
}

constexpr int Inverse(int x, int prime_mod) {
    return ModularExp(x, prime_mod - 2, prime_mod);
}

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_MODULAR_EXP_HPP_