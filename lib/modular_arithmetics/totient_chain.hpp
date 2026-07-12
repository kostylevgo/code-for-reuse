#ifndef TOTIENT_CHAIN_HPP_
#define TOTIENT_CHAIN_HPP_

#include "number_theory/utility.hpp"

#include <tuple>
#include <array>

constexpr auto NextItem(int v) {
    constexpr int p = Phi(v);
    if constexpr (v % p == 0) {
        return std::tuple{};
    } else {
        return std::tuple{p};
    }
}

template <int MOD>
struct TotientChain {

};

#endif