#pragma once

#include "mexp.hpp"

#include <bits/stdc++.h>

using namespace std;

inline int inverse(int x, int prime_mod) {
    return mexp(x, prime_mod - 2, prime_mod);
}

inline int discrete_logarithm(int x, int base, int prime_mod) {
    if (x == 1) return 0;

    int phi = prime_mod - 1;
    int giant_step = (int)sqrt(phi);

    auto make_steps = [&](int start, int64_t step, int cnt) {
        vector<pair<int, int>> ans;
        int pw = start;
        for (int i = 0; i < cnt; ++i) {
            ans.emplace_back(pw, i);
            pw = pw * step % prime_mod;
        }
        return ans;
    };

    vector<pair<int, int>> baby_steps = make_steps(1, base, giant_step);
    sort(baby_steps.begin(), baby_steps.end());

    int giant_base = mexp(base, giant_step, prime_mod);
    vector<pair<int, int>> giant_steps = make_steps(giant_base, giant_base, (phi - 2) / giant_step + 1);

    int64_t inv_x = inverse(x, prime_mod);
    for (auto [giant, giant_pw] : giant_steps) {
        int excess = inv_x * giant % prime_mod;
        int ind = (int)(ranges::lower_bound(baby_steps, pair{excess, -1}) - baby_steps.begin());
        if (ind != baby_steps.size() && baby_steps[ind].first == excess) {
            return (giant_pw * giant_step + giant_step - baby_steps[ind].second) % phi;
        }
    }
    assert(false);
}