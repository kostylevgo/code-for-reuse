#ifndef PHI_SIEVE_HPP
#define PHI_SIEVE_HPP

#include "utility.hpp"
#include "sieve.hpp"

template <int SIEVE>
int Phi(int n, const Sieve<SIEVE>& sieve) {
    return PhiBase(n, [&sieve](int x) {return sieve.Factorize(x);});
}

#endif
