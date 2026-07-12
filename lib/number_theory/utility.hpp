#ifndef NUMBER_THEORY_UTILITY_HPP_
#define NUMBER_THEORY_UTILITY_HPP_

#include <vector>
#include <utility>

constexpr int ModularExp(int x, int64_t y, int mod) {
    int c = 1;
    while (y) {
        if (y & 1) c = int64_t{c} * x % mod;
        y >>= 1;
        x = int64_t{x} * x % mod;
    }
    return c;
}

constexpr int Inverse(int x, int prime_mod) {
    return ModularExp(x, prime_mod - 2, prime_mod);
}

constexpr int IsPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

constexpr std::vector<std::pair<int, int>> Factorize(int x) {
    std::vector<std::pair<int, int>> ans;
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

constexpr int PhiBase(int n, auto&& factorize) {
    int ans = n;
    for (auto [div, _] : factorize(n)) {
        ans /= div;
        ans *= div - 1;
    }
    return ans;
}

constexpr int Phi(int n) {
    return PhiBase(n, Factorize);
}

#endif