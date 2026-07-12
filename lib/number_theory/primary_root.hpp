#ifndef PRIMARY_ROOT_HPP_
#define PRIMARY_ROOT_HPP_

#include <utility>
#include <vector>

#include "utility.hpp"

constexpr int PrimaryRootBase(int p, auto&& factorize) {
    if (p == 2) return 1;
    int phi = p - 1;
    std::vector<std::pair<int, int>> divs;
    divs = factorize(phi); // or use sieve
    for (int r = 2; r < p; ++r) {
        for (auto [div, _] : divs) {
            int test = ModularExp(r, phi / div, p);
            if (test == 1) {
                goto next;
            }
        }
        return r;
        next:
    }
    return -1;
}

constexpr int PrimaryRoot(int p) {
    return PrimaryRootBase(p, Factorize);
}

#endif