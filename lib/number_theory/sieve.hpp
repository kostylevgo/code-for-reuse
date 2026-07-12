#ifndef SIEVE_HPP_
#define SIEVE_HPP_

#include <array>
#include <algorithm>
#include <vector>
#include <utility>

#include "utility.hpp"

template <int SIEVE>
class Sieve {
public:
    constexpr Sieve() {
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

    [[nodiscard]] constexpr bool IsPrime(int x) const {
        return x >= 2 && low_div[x] == x;
    }

    [[nodiscard]] constexpr std::vector<std::pair<int, int>> Factorize(int x) const {
        std::vector<std::pair<int, int>> ans;
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
    std::array<int, SIEVE> low_div;
    std::array<int, std::max(10'000, SIEVE / 10)> primes;
    int pr_sz = 0;
};

#endif
