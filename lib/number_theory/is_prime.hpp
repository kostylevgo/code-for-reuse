#ifndef CODE_FOR_REUSE_NUMBER_THEORY_IS_PRIME_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_IS_PRIME_HPP_

#include "factorize.hpp"

template <typename T = Factorize>
constexpr bool IsPrime(auto n, T&& factor = Factorize{}) {
    if (n < 2) {
        return false;
    }
    bool is_first = true;
    for (auto [div, cnt] : factor(n)) {
        if (cnt >= 2) {
            return false;
        }
        if (is_first) {
            is_first = false;
        } else {
            return false;
        }
    }
    return true;
}

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_IS_PRIME_HPP_