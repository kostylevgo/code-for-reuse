#ifndef CODE_FOR_REUSE_NUMBER_THEORY_PRIMARY_ROOT_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_PRIMARY_ROOT_HPP_

#include <vector>

#include "factorize.hpp"
#include "modular_exp.hpp"

template <typename T = Factorize>
constexpr int PrimaryRoot(int p, T&& factor = Factorize{}) {
    if (p == 2) return 1;
    int phi = p - 1;
    std::vector divs(std::from_range, factor(phi));
    for (int r = 2; r < p; ++r) {
        bool is_successful = true;
        for (auto [div, _] : divs) {
            int test = ModularExp(r, phi / div, p);
            if (test == 1) {
                is_successful = false;
                break;
            }
        }
        if (is_successful) {
            return r;
        }
    }
    return -1;
}

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_PRIMARY_ROOT_HPP_