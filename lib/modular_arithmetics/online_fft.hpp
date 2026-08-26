#ifndef CODE_FOR_REUSE_MODULAR_ARITHMETICS_ONLINE_FFT_HPP_
#define CODE_FOR_REUSE_MODULAR_ARITHMETICS_ONLINE_FFT_HPP_

#include <unordered_map>

#include "fft.hpp"

auto Square(auto x) {
    return x * x;
}

template <Mint mint>
struct OnlineFFT {
    std::vector<mint> a;
    std::vector<mint> b;
    std::vector<mint> a_mul_b;
    std::unordered_map<int, mint> a_queries;
    std::unordered_map<int, mint> b_queries;

    mint At(int i) {
        mint ans = (i < a_mul_b.size() ? a_mul_b[i] : 0);
        for (int it = 0; it < 2; ++it, SwapAB()) {
            for (auto [a_i, delta] : a_queries) {
                int b_i = i - a_i;
                if (b_i >= 0 && b_i < b.size()) {
                    ans += delta * b[b_i];
                }
            }
        }
        for (auto [a_i, delta] : a_queries) {
            int b_i = i - a_i;
            auto it = b_queries.find(b_i);
            if (it != b_queries.end()) {
                ans += delta * it->second;
            }
        }
        return ans;
    }

    void AddQueryA(int pos, mint delta) {
        a_queries[pos] += delta;
        FlushIfNeeded();
    }

    void AddQueryB(int pos, mint delta) {
        b_queries[pos] += delta;
        FlushIfNeeded();
    }

    void FlushIfNeeded() {
        if (Square(a_queries.size() + b_queries.size()) > 5 * (a.size() + b.size()) * Log(a.size() + b.size())) {
            Flush();
        }
    }

    void Flush() {
        for (int it = 0; it < 2; ++it, SwapAB()) {
            for (auto [i, delta] : a_queries) {
                if (a.size() <= i) {
                    a.resize(i + 1);
                }
                a[i] += delta;
            }
            a_queries.clear();
        }
        a_mul_b = Multiply(a, b);
    }

    void SwapAB() {
        swap(a, b);
        swap(a_queries, b_queries);
    }
};

#endif  // CODE_FOR_REUSE_MODULAR_ARITHMETICS_ONLINE_FFT_HPP_