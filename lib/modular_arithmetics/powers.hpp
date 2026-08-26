#ifndef CODE_FOR_REUSE_MODULAR_ARITHMETICS_POWERS_HPP_
#define CODE_FOR_REUSE_MODULAR_ARITHMETICS_POWERS_HPP_

#include <array>

#include "modular.hpp"

template <Mint mint, int N>
class Powers {
  public:
    explicit Powers(mint B) {
        powers_[0] = 1;
        for (int i = 1; i < N; ++i) {
            powers_[i] = powers_[i - 1] * B;
        }
    }

    mint Power(int p) {
        if (p < N) {
            return powers_[p];
        }
        int full = p / (N - 1);
        return mint::Exp(powers_[p], full) * powers_[p - full * (N - 1)];
    }

  private:
    std::array<mint, N> powers_{};
};

#endif  // CODE_FOR_REUSE_MODULAR_ARITHMETICS_POWERS_HPP_