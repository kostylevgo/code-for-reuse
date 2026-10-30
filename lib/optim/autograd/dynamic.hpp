#ifndef CODE_FOR_REUSE_OPTIM_AUTOGRAD_DYNAMIC_HPP_
#define CODE_FOR_REUSE_OPTIM_AUTOGRAD_DYNAMIC_HPP_

#include <vector>

template <class ld = long double>
struct DynamicAutograd {
    ld x;
    std::vector<ld> grads;

    DynamicAutograd(int n, ld x) : x(x), grads(n) {
    }

    DynamicAutograd() : DynamicAutograd(0) {
    }

    static DynamicAutograd variable(ld value, int i) {
        DynamicAutograd ans(value);
        ans.grads[i] = 1;
        return ans;
    }

    DynamicAutograd& operator+=(DynamicAutograd b) {
        x += b.x;
        for (int i = 0; i < grads.size(); ++i) {
            grads[i] += b.grads[i];
        }
        return *this;
    }

    DynamicAutograd operator-() {
        DynamicAutograd ans = *this;
        ans.x = -ans.x;
        for (auto& v : ans.grads) {
            v = -v;
        }
        return ans;
    }

    DynamicAutograd& operator-=(DynamicAutograd b) {
        b = -b;
        return *this += b;
    }

    DynamicAutograd& operator*=(DynamicAutograd b) {
        for (int i = 0; i < grads.size(); ++i) {
            grads[i] = grads[i] * b.x + b.grads[i] * x;
        }
        x *= b.x;
        return *this;
    }

    DynamicAutograd& operator/=(DynamicAutograd b) {
        for (int i = 0; i < grads.size(); ++i) {
            grads[i] = grads[i] / b.x - x * b.grads[i] / b.x / b.x;
        }
        x /= b.x;
        return *this;
    }

    friend DynamicAutograd operator+(DynamicAutograd a, DynamicAutograd b) {
        return a += b;
    }

    friend DynamicAutograd operator-(DynamicAutograd a, DynamicAutograd b) {
        return a -= b;
    }

    friend DynamicAutograd operator*(DynamicAutograd a, DynamicAutograd b) {
        return a *= b;
    }

    friend DynamicAutograd operator/(DynamicAutograd a, DynamicAutograd b) {
        return a /= b;
    }

    friend DynamicAutograd sqrt(DynamicAutograd v) {
        ld s = sqrt(v.x);
        for (int i = 0; i < v.grads.size(); ++i) {
            v.grads[i] /= 2 * s;
        }
        v.x = s;
        return v;
    }
};

#endif  // CODE_FOR_REUSE_OPTIM_AUTOGRAD_DYNAMIC_HPP_