#ifndef CODE_FOR_REUSE_NUMBER_THEORY_DISCRETE_SQRT_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_DISCRETE_SQRT_HPP_

#include "discrete_log.hpp"
#include "factorize.hpp"
#include "modular_exp.hpp"
#include "primary_root.hpp"

template <typename T = Factorize>
std::pair<int, int> DiscreteSqrt(int x, int prime_mod, T&& factor = Factorize{}) {
    if (prime_mod == 2) {
        return {1, -1};
    }
    int root = PrimaryRoot(prime_mod, factor);
    int l = DiscreteLogarithm(x, root, prime_mod);
    if (l % 2 == 0) {
        return {ModularExp(root, l / 2, prime_mod), ModularExp(root, (l + prime_mod - 1) / 2, prime_mod)};
    }
    return {-1, -1};
}

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_DISCRETE_SQRT_HPP_