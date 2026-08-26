#include "modular.hpp"

#include "number_theory/crt.hpp"

template <Mint m1, Mint m2>
int64_t crt(m1 a, m2 b) {
    return crt(a.v, m1::kMod, b.v, m2::kMod);
}
