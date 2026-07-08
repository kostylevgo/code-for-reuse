#pragma once

#include "mexp.hpp"

#include <bits/stdc++.h>

using namespace std;

inline vector<pair<int, int>> factorize(int x) {
    vector<pair<int, int>> ans;
    for (int i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            int pw = 0;
            do {
                ++pw;
                x /= i;
            } while (x % i == 0);
            ans.emplace_back(i, pw);
        }
    }
    if (x > 1) ans.emplace_back(x, 1);
    return ans;
}

inline int primary_root(int p) {
    if (p == 2) return 1;
    int phi = p - 1;
    vector<pair<int, int>> divs;
    divs = factorize(phi); // or use sieve
    for (int r = 2; r < p; ++r) {
        for (auto [div, _] : divs) {
            int test = mexp(r, phi / div, p);
            if (test == 1) {
                goto next;
            }
        }
        return r;
        next:
    }
    return -1;
}
