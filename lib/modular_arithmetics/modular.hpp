#ifndef MODULAR_HPP_
#define MODULAR_HPP_

#include <bits/stdc++.h>

using namespace std;

template <int M>
struct Modular {
    int v;

    constexpr Modular() : v(0) {}

    constexpr Modular(const int64_t x) : v(x < 0 ? x % M + M : x % M) {}

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

    friend Modular operator+(Modular a, Modular b) { return a += b; }

    friend Modular operator-(Modular a, Modular b) { return a -= b; }

    friend Modular operator*(Modular a, Modular b) { return a *= b; }

    friend Modular operator-(Modular a) { return 0 - a; }

    static Modular Exp(Modular x, int64_t y) {
        Modular c = 1;
        while (y) {
            if (y & 1) c *= x;
            y >>= 1, x *= x;
        }
        return c;
    }

    static Modular Inverse(const Modular x) { return Exp(x, M - 2); }

    friend istream& operator>>(istream& is, Modular& a) { return is >> a.v; }
    friend ostream& operator<<(ostream& os, Modular a) { return os << a.v; }

    bool operator==(const Modular&) const = default;
    std::strong_ordering operator<=>(const Modular&) const = default;
};

namespace std {

template <int N>
struct hash<Modular<N>> {
    size_t operator()(const Modular<N>& x) const { return x.v; }
};

}  // namespace std

constexpr int MOD = 998'244'353;
using mint = Modular<MOD>;

#endif