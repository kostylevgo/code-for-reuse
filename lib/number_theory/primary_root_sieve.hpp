#ifndef PRIMARY_ROOT_SIEVE_HPP
#define PRIMARY_ROOT_SIEVE_HPP

#include "sieve.hpp"
#include "primary_root.hpp"

template <int SIEVE>
constexpr int PrimaryRoot(int p, const Sieve<SIEVE>& sieve) {
    return PrimaryRootBase(p, [&sieve](int x) {return sieve.Factorize(x);});
}

#endif
