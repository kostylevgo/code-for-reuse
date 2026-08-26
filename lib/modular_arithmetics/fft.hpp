#ifndef CODE_FOR_REUSE_MODULAR_ARITHMETICS_FFT_HPP_
#define CODE_FOR_REUSE_MODULAR_ARITHMETICS_FFT_HPP_

#include <algorithm>
#include <bit>
#include <vector>

#include "primary_root.hpp"

int Log(int size) {
    if (size == 0) {
        return -1;
    }
    return std::bit_width<uint32_t>(size - 1);
}

template <Mint mint>
mint Root(int log) {
    return mint::Exp(kRoot<mint::kMod>, (mint::kMod - 1) >> log);
}

uint32_t Reverse(uint32_t x, int n) {
    x = ((x & 0x55555555u) << 1) | ((x & 0xAAAAAAAAu) >> 1);
    x = ((x & 0x33333333u) << 2) | ((x & 0xCCCCCCCCu) >> 2);
    x = ((x & 0x0F0F0F0Fu) << 4) | ((x & 0xF0F0F0F0u) >> 4);
    x = ((x & 0x00FF00FFu) << 8) | ((x & 0xFF00FF00u) >> 8);
    x = ((x & 0x0000FFFFu) << 16) | ((x & 0xFFFF0000u) >> 16);
    return x >> (32 - n);
}

template <Mint mint>
void Butterfly(std::vector<mint>& v) {
    int log = Log(v.size());
    if (log <= 1) {
        return;
    }
    for (int i = 0; i < v.size(); ++i) {
        int j = Reverse(i, log);
        if (j > i) {
            swap(v[i], v[j]);
        }
    }
}

template <Mint mint>
void DFTOnButterflied(std::vector<mint>& v) {
    for (int i = 0; (1 << i) < v.size(); ++i) {
        int n = 1 << i;
        mint step = Root<mint>(i + 1);
        for (int j = 0; j < v.size(); j += 2 * n) {
            mint x = 1;
            for (int k = 0; k < n; ++k) {
                mint& a = v[j + k];
                mint& b = v[j + n + k];
                mint new_a = a + b * x;
                mint new_b = a - b * x;
                a = new_a;
                b = new_b;
                x *= step;
            }
        }
    }
}

template <Mint mint>
void DFT(std::vector<mint>& v) {
    Butterfly(v);
    DFTOnButterflied(v);
}

template <Mint mint>
void InverseDFT(std::vector<mint>& v) {
    DFT(v);
    std::reverse(v.begin() + 1, v.end());
    mint inv = mint::Inverse(v.size());
    for (auto& x : v) {
        x *= inv;
    }
}

template <Mint mint>
std::vector<mint> Multiply(std::vector<mint> a, std::vector<mint> b) {
    if (a.empty() || b.empty()) {
        return {};
    }
    int result_size = a.size() + b.size() - 1;
    int log = Log(result_size);
    int size = 1 << log;
    a.resize(size);
    b.resize(size);
    DFT(a);
    DFT(b);
    for (int i = 0; i < size; ++i) {
        a[i] *= b[i];
    }
    InverseDFT(a);
    a.resize(result_size);
    return a;
}

#endif  // CODE_FOR_REUSE_MODULAR_ARITHMETICS_FFT_HPP_