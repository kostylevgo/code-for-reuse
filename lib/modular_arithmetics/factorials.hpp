#pragma once

#include "modular.hpp"

#include <bits/stdc++.h>

using namespace std;

template <int N, int MOD = MOD>
class Factorials {
    using mint = Modular<MOD>;

public:
    Factorials() {
        factorials_[0] = 1;
        for (int i = 1; i < N; ++i) {
            factorials_[i] = factorials_[i - 1] * i;
        }
        inverse_factorials_[N - 1] = mint::Inverse(factorials_[N - 1]);
        for (int i = N - 1; i >= 1; --i) {
            inverse_factorials_[i - 1] = inverse_factorials_[i] * i;
        }
    }

    mint Binom(int n, int k) const {
        return C(n, k);
    }

    mint C(int n, int k) const {
        if (k < 0 || k > n) {
            return 0;
        }
        return Factorial(n) * InverseFactorial(k) * InverseFactorial(n - k);
    }

    mint Inverse(int x) const {
        return InverseFactorial(x) * Factorial(x - 1);
    }

    mint operator()(int x) const {
        return Factorial(x);
    }

    mint Factorial(int x) const {
        return factorials_[x];
    }

    mint InverseFactorial(int x) const {
        return inverse_factorials_[x];
    }

private:
    array<mint, N> factorials_{};
    array<mint, N> inverse_factorials_{};
};
