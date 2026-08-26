#ifndef CODE_FOR_REUSE_MODULAR_ARITHMETICS_PRIMARY_ROOT_HPP_
#define CODE_FOR_REUSE_MODULAR_ARITHMETICS_PRIMARY_ROOT_HPP_

#include "modular.hpp"
#include "number_theory/primary_root.hpp"

template <int MOD>
static constexpr Modular<MOD> kRoot = PrimaryRoot(MOD);

#endif  // CODE_FOR_REUSE_MODULAR_ARITHMETICS_PRIMARY_ROOT_HPP_