#pragma once

#include <bits/stdc++.h>

using namespace std;

inline int mexp(int x, int64_t y, int mod) {
    int c = 1;
    while (y) {
        if (y & 1) c = int64_t{c} * x % mod;
        y >>= 1;
        x = int64_t{x} * x % mod;
    }
    return c;
}