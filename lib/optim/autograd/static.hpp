#ifndef CODE_FOR_REUSE_OPTIM_AUTOGRAD_STATIC_HPP_
#define CODE_FOR_REUSE_OPTIM_AUTOGRAD_STATIC_HPP_

#include <array>

template <int N, class ld = long double>
struct Autograd {
    ld x;
    std::array<ld, N> grads{};

    Autograd(ld x) : x(x) {
    }

    Autograd() : Autograd(0) {
    }

    static Autograd variable(ld value, int i) {
        Autograd ans(value);
        ans.grads[i] = 1;
        return ans;
    }

    Autograd& operator+=(Autograd b) {
        x += b.x;
        for (int i = 0; i < N; ++i) {
            grads[i] += b.grads[i];
        }
        return *this;
    }

    Autograd operator-() {
        Autograd ans = *this;
        ans.x = -ans.x;
        for (auto& v : ans.grads) {
            v = -v;
        }
        return ans;
    }

    Autograd& operator-=(Autograd b) {
        b = -b;
        return *this += b;
    }

    Autograd& operator*=(Autograd b) {
        for (int i = 0; i < N; ++i) {
            grads[i] = grads[i] * b.x + b.grads[i] * x;
        }
        x *= b.x;
        return *this;
    }

    Autograd& operator/=(Autograd b) {
        for (int i = 0; i < N; ++i) {
            grads[i] = grads[i] / b.x - x * b.grads[i] / b.x / b.x;
        }
        x /= b.x;
        return *this;
    }

    friend Autograd operator+(Autograd a, Autograd b) {
        return a += b;
    }

    friend Autograd operator-(Autograd a, Autograd b) {
        return a -= b;
    }

    friend Autograd operator*(Autograd a, Autograd b) {
        return a *= b;
    }

    friend Autograd operator/(Autograd a, Autograd b) {
        return a /= b;
    }

    friend Autograd sqrt(Autograd v) {
        ld s = sqrt(v.x);
        for (int i = 0; i < N; ++i) {
            v.grads[i] /= 2 * s;
        }
        v.x = s;
        return v;
    }
};

#endif  // CODE_FOR_REUSE_OPTIM_AUTOGRAD_STATIC_HPP_