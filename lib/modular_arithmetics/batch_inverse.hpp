#include "modular.hpp"

#include <vector>

template <Mint mint>
void Inverse(std::vector<mint>& v) {
    std::vector<mint> factorials(v.size() + 1);
    factorials[0] = 1;
    for (int i = 0; i < v.size(); ++i) {
        factorials[i + 1] = factorials[i] * v[i];
    }
    std::vector<mint> inverse_factorials(v.size() + 1);
    inverse_factorials.back() = mint::Inverse(factorials.back());
    for (int i = v.size(); i--;) {
        inverse_factorials[i] = v[i] * inverse_factorials[i + 1];
    }
    for (int i = 0; i < v.size(); ++i) {
        v[i] = inverse_factorials[i + 1] * factorials[i];
    }
}
