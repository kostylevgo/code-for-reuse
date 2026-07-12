#ifndef CODE_FOR_REUSE_MODULAR_ARITHMETICS_KAPT_INVERTER_HPP_
#define CODE_FOR_REUSE_MODULAR_ARITHMETICS_KAPT_INVERTER_HPP_

#include <vector>

#include "factorials.hpp"

template <int MOD>
class Inverter {
    using mint = Modular<MOD>;

    struct Inverted {
        Inverter& base;
        int index;

        operator mint() const {
            return base.GetInverted(index);
        }
    };

  public:
    void Reserve(size_t n) {
        mult_.reserve(n);
        mult_inv_.reserve(n);
    }

    Inverted Inverse(mint x) {
        Inverted ans{*this, static_cast<int>(mult_.size())};
        mult_inv_.push_back(x);
        mult_.push_back(x * mult_.back());
        return ans;
    }

  private:
    void Update(int to) {
        if (to < actual_size) {
            return;
        }
        mint total_rev = mint::Inverse(mult_.back());
        for (int i = static_cast<int>(mult_inv_.size()) - 1; i >= actual_size; --i) {
            mint new_rev = total_rev * mult_inv_[i];
            mult_inv_[i] = total_rev;
            total_rev = new_rev;
        }
        actual_size = mult_.size();
    }

    mint GetInverted(int index) {
        Update(index);
        return mult_inv_[index] * mult_[index - 1];
    }

  private:
    std::vector<mint> mult_{1};
    std::vector<mint> mult_inv_{1};
    int actual_size = 1;
};

// int main() {
//     Inverter a;
//     auto inv_2 = a.inverse(2);
//     cout << (mint)inv_2 << endl;
// }

#endif  // CODE_FOR_REUSE_MODULAR_ARITHMETICS_KAPT_INVERTER_HPP_