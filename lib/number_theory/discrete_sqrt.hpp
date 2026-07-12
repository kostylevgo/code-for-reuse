#pragma once

#include "discrete_log.hpp"
#include "primary_root.hpp"

inline std::pair<int, int> DiscreteSqrt(int x, int prime_mod) {
    if (prime_mod == 2) {
        return {1, -1};
    }
    int root = PrimaryRoot(prime_mod);
    int l = DiscreteLogarithm(x, root, prime_mod);
    if (l % 2 == 0) {
        return {ModularExp(root, l / 2, prime_mod), ModularExp(root, (l + prime_mod - 1) / 2, prime_mod)};
    }
    return {-1, -1};
}