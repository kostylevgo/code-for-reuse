#ifndef CODE_FOR_REUSE_NUMBER_THEORY_SIEVE_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_SIEVE_HPP_

#include <algorithm>
#include <array>
#include <ranges>

#include "structs.hpp"

template <int SIEVE>
class Sieve {
  public:
    struct Sentinel {};

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = ptrdiff_t;

        constexpr Iterator() = default;

        constexpr Iterator(int x, const Sieve& s) : x(x), sieve(&x) {
            Step();
        }

        constexpr Iterator& operator++() {
            Step();
            return *this;
        }

        constexpr Iterator operator++(int) {
            Iterator v = *this;
            ++*this;
            return v;
        }

        constexpr const Divisor<int>* operator->() const {
            return &cur;
        }

        constexpr const Divisor<int>& operator*() const {
            return cur;
        }

        constexpr bool operator==(Sentinel) const {
            return x == 1 && cur == Divisor<int>{};
        }

      private:
        int x = 1;
        Divisor<int> cur{};
        const Sieve* sieve = nullptr;

      private:
        constexpr void Step() {
            if (x == 1) {
                cur = Divisor<int>{};
                return;
            }
            cur.divisor = sieve->lowest_divisor[x];
            cur.power = 0;
            while (x % cur.divisor == 0) {
                x /= cur.divisor;
                ++cur.power;
            }
        }
    };

    constexpr Sieve() {
        for (int i = 2; i < SIEVE; ++i) {
            if (lowest_divisor[i] == 0) {
                lowest_divisor[i] = i;
                primes[prime_count++] = i;
            }
            for (int j = 0; j < prime_count && primes[j] <= lowest_divisor[i] && primes[j] * i < SIEVE; ++j) {
                lowest_divisor[i * primes[j]] = primes[j];
            }
        }
    }

    Sieve(const Sieve&) = delete;
    Sieve& operator=(const Sieve&) = delete;
    Sieve(Sieve&&) = delete;
    Sieve& operator=(Sieve&&) = delete;

    [[nodiscard]] constexpr auto Factorize(int x) const {
        return std::ranges::subrange(Iterator(x, *this), Sentinel{});
    }

    [[nodiscard]] constexpr auto operator()(int x) const {
        return Factorize(x);
    }

    std::array<int, SIEVE> lowest_divisor{};
    std::array<int, std::max(10'000, SIEVE / 10)> primes{};
    int prime_count = 0;
};

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_SIEVE_HPP_