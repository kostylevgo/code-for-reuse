#pragma once

#include <bits/stdc++.h>

using namespace std;

template <int SIEVE>
class Sieve {
public:
    Sieve() {
        for (int i = 2; i < SIEVE; ++i) {
            if (low_div[i] == 0) {
                low_div[i] = i;
                primes[pr_sz++] = i;
            }
            for (int j = 0; j < pr_sz && primes[j] <= low_div[i] && primes[j] * i < SIEVE; ++j) {
                low_div[i * primes[j]] = primes[j];
            }
        }
    }

    bool is_prime(int x) {
        return x >= 2 && low_div[x] == x;
    }

    vector<pair<int, int>> factorize(int x) {
        vector<pair<int, int>> ans;
        while (x > 1) {
            int div = low_div[x], pw = 0;
            while (x % div == 0) {
                ++pw;
                x /= div;
            }
            ans.emplace_back(div, pw);
        }
        return ans;
    }

private:
    array<int, SIEVE> low_div;
    array<int, max(10'000, SIEVE / 10)> primes;
    int pr_sz = 0;
};
