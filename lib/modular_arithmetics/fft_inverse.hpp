#ifndef CODE_FOR_REUSE_MODULAR_ARITHMETICS_FFT_INVERSE_HPP_
#define CODE_FOR_REUSE_MODULAR_ARITHMETICS_FFT_INVERSE_HPP_

#include "fft.hpp"

template <Mint mint>
std::vector<mint> Inverse(std::vector<mint> a, int n) {
    if (n == 0) {
        return {};
    }
    int log = Log(n);
    int size = 1 << log;
    a.resize(size);
    std::vector<mint> result(size);
    result[0] = mint::Inverse(a[0]);
    for (int i = 0; i < log; ++i) {
        int len = 1 << i;
        std::vector b(a.begin() + len, a.begin() + 2 * len);
        std::vector d(result.begin(), result.begin() + len);
        std::vector d_inv(a.begin(), a.begin() + len);
        std::vector top_half = Multiply(d_inv, d);
        top_half.resize(2 * len);
        top_half.erase(top_half.begin(), top_half.begin() + len);
        b = Multiply(std::move(b), d);
        b.resize(len);
        for (int j = 0; j < len; ++j) {
            b[j] += top_half[j];
        }
        b = Multiply(std::move(b), std::move(d));
        for (int j = 0; j < len; ++j) {
            result[j + len] = -b[j];
        }
    }
    result.resize(n);
    return result;
}

#endif  // CODE_FOR_REUSE_MODULAR_ARITHMETICS_FFT_INVERSE_HPP_