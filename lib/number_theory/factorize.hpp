#ifndef CODE_FOR_REUSE_NUMBER_THEORY_FACTORIZE_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_FACTORIZE_HPP_

#include <algorithm>
#include <iterator>
#include <ranges>

#include "structs.hpp"

struct FactorizeSentinel {};

template <typename T>
struct FactorizeIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = ptrdiff_t;

    constexpr FactorizeIterator() = default;

    constexpr explicit FactorizeIterator(T x) : x(std::max<T>(1, x)) {
        cur.divisor = 2;
        Step();
    }

    constexpr FactorizeIterator& operator++() {
        Step();
        return *this;
    }

    constexpr FactorizeIterator operator++(int) {
        FactorizeIterator v = *this;
        ++*this;
        return v;
    }

    constexpr const Divisor<T>* operator->() const {
        return &cur;
    }

    constexpr const Divisor<T>& operator*() const {
        return cur;
    }

    constexpr bool operator==(FactorizeSentinel) const {
        return x == 1 && cur == Divisor<T>{};
    }

  private:
    T x = 1;
    Divisor<T> cur{};

  private:
    constexpr void Step() {
        if (x == 1) {
            cur = Divisor<T>{};
            return;
        }
        while (cur.divisor * cur.divisor <= x && x % cur.divisor != 0) {
            ++cur.divisor;
        }
        if (cur.divisor * cur.divisor > x) {
            cur.divisor = x;
        }
        cur.power = 0;
        do {
            x /= cur.divisor;
            ++cur.power;
        } while (x % cur.divisor == 0);
    }
};

struct Factorize {
    constexpr auto operator()(auto n) const {
        return std::ranges::subrange(FactorizeIterator(n), FactorizeSentinel{});
    }
};

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_FACTORIZE_HPP_