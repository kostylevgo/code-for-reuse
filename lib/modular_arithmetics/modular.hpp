#ifndef CODE_FOR_REUSE_MODULAR_ARITHMETICS_MODULAR_HPP_
#define CODE_FOR_REUSE_MODULAR_ARITHMETICS_MODULAR_HPP_

#include <compare>
#include <cstdint>
#include <functional>
#include <iostream>

template <int M>
struct Modular {
    int v;

    constexpr Modular() : v(0) {
    }

    constexpr Modular(int64_t x) : v(x < 0 ? x % M + M : x % M) {
    }

    Modular& operator+=(Modular a) {
        v += a.v;
        if (v >= M) v -= M;
        return *this;
    }

    Modular& operator-=(Modular a) {
        v -= a.v;
        if (v < 0) v += M;
        return *this;
    }

    Modular& operator*=(Modular a) {
        v = 1ll * v * a.v % M;
        return *this;
    }

    friend Modular operator+(Modular a, Modular b) {
        return a += b;
    }

    friend Modular operator-(Modular a, Modular b) {
        return a -= b;
    }

    friend Modular operator*(Modular a, Modular b) {
        return a *= b;
    }

    friend Modular operator-(Modular a) {
        return 0 - a;
    }

    static Modular Exp(Modular x, int64_t y) {
        Modular c = 1;
        while (y) {
            if (y & 1) c *= x;
            y >>= 1, x *= x;
        }
        return c;
    }

    static Modular Inverse(const Modular x) {
        return Exp(x, M - 2);
    }

    friend std::istream& operator>>(std::istream& is, Modular& a) {
        int64_t x;
        is >> x;
        a = Modular{x};
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, Modular a) {
        return os << a.v;
    }

    std::strong_ordering operator<=>(const Modular&) const = default;
    bool operator==(const Modular&) const = default;
};

template <int N>
struct std::hash<Modular<N>> {
    size_t operator()(const Modular<N>& x) const {
        return x.v;
    }
};

#endif  // CODE_FOR_REUSE_MODULAR_ARITHMETICS_MODULAR_HPP_