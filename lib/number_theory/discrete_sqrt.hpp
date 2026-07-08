#pragma once

#include "discrete_log.hpp"
#include "primary_root.hpp"

#include <bits/stdc++.h>

using namespace std;

inline pair<int, int> discrete_sqrt(int x, int prime_mod) {
    if (prime_mod == 2) {
        return {1, -1};
    }
    int root = primary_root(prime_mod);
    int l = discrete_logarithm(x, root, prime_mod);
    if (l % 2 == 0) {
        return {mexp(root, l / 2, prime_mod), mexp(root, (l + prime_mod - 1) / 2, prime_mod)};
    }
    return {-1, -1};
}