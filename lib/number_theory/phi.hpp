#ifndef CODE_FOR_REUSE_NUMBER_THEORY_PHI_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_PHI_HPP_

#include "factorize.hpp"

template <typename T = Factorize>
constexpr int Phi(int n, T&& factor = Factorize{}) {
    int ans = n;
    for (auto [div, _] : factor(n)) {
        ans /= div;
        ans *= div - 1;
    }
    return ans;
}

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_PHI_HPP_