#include "modular.hpp"
#include "number_theory/primary_root.hpp"

template <int MOD>
static constexpr Modular<MOD> kRoot = PrimaryRoot(MOD);
