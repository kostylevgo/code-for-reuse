#ifndef CODE_FOR_REUSE_MODULAR_ARITHMETICS_TOTIENT_CHAIN_HPP_
#define CODE_FOR_REUSE_MODULAR_ARITHMETICS_TOTIENT_CHAIN_HPP_

#include "../number_theory/is_prime.hpp"
#include "../number_theory/phi.hpp"
#include "../strings/multihash.hpp"

template <>
struct Modular<0> {
    static constexpr int MAX = std::numeric_limits<int>::max() / 2;

    uint32_t v = 0;

    Modular() = default;

    Modular(int64_t x) : v(Check(x)) {
    }

    Modular& operator+=(Modular other) {
        v = Check(v + other.v);
        return *this;
    }

    Modular& operator-=(Modular other) {
        if (*this) {
            return *this;
        }
        v -= other.v;
        return *this;
    }

    Modular& operator*=(Modular other) {
        v = Check(int64_t{v} * other.v);
        return *this;
    }

    friend Modular operator+(Modular a, Modular b) {
        return a += b;
    }

    friend Modular operator-(Modular a, Modular b) {
        return a -= b;
    }

    friend Modular operator*(Modular a, Modular b) {
        return a *= b;
    }

    bool IsCap() const {
        return v == MAX;
    }

    explicit operator bool() const {
        return IsCap();
    }

    static Modular Exp(Modular x, int y) {
        Modular c = 1;
        while (y) {
            if (y & 1) {
                c *= x;
            }
            y >>= 1;
            x *= x;
        }
        return c;
    }

    std::strong_ordering operator<=>(const Modular&) const = default;
    bool operator==(const Modular&) const = default;

    friend std::istream& operator>>(std::istream& is, Modular& x) {
        int64_t k;
        is >> k;
        x = Modular{k};
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, Modular x) {
        return os << x.v;
    }

  private:
    static int Check(auto v) {
        if (v >= MAX) {
            v = MAX;
        }
        return v;
    }
};

template <int M>
struct ComputeChainBaseHelper {
    template <typename T, std::size_t... Sizes>
    static constexpr auto ConcatenateArrays(const std::array<T, Sizes>&... arrays) {
        return std::apply([](auto... elements) { return std::array<T, (Sizes + ...)>{elements...}; },
                          std::tuple_cat(arrays...));
    }

    template <int MOD>
    static constexpr auto ComputeModArray() {
        constexpr int phi = Phi(MOD);
        if constexpr (MOD % phi == 0) {
            return std::array{MOD, 0};
        } else {
            return ConcatenateArrays(std::array{MOD}, ComputeModArray<phi>());
        }
    }

    template <auto MODS, std::size_t... Is>
    static constexpr auto ComputeChainTypeHelper(std::index_sequence<Is...>) {
        return HashValue<MODS[Is]...>{};
    }

    template <auto MODS>
    static constexpr auto ComputeChainType() {
        constexpr std::size_t N = std::tuple_size_v<decltype(MODS)>;
        return ComputeChainTypeHelper<MODS>(std::make_index_sequence<N>{});
    }

    static constexpr auto MS = ComputeModArray<M>();
    using Base = decltype(ComputeChainType<MS>());
};

template <int MOD>
struct ChainValue {
    static_assert(IsPrime(MOD));
    static constexpr auto MODS = ComputeChainBaseHelper<MOD>::MS;
    static constexpr size_t MOD_COUNT = std::tuple_size_v<decltype(MODS)>;
    using Base = ComputeChainBaseHelper<MOD>::Base;

    Base v;

    ChainValue() = default;

    ChainValue(int64_t x) : v(x) {
    }

    ChainValue& operator+=(const ChainValue& other) {
        v += other.v;
        return *this;
    }

    ChainValue& operator-=(const ChainValue& other) {
        v -= other.v;
        return *this;
    }

    ChainValue& operator*=(const ChainValue& other) {
        v *= other.v;
        return *this;
    }

    friend ChainValue operator+(ChainValue lhs, const ChainValue& rhs) {
        return lhs += rhs;
    }

    friend ChainValue operator-(ChainValue lhs, const ChainValue& rhs) {
        return lhs -= rhs;
    }

    friend ChainValue operator*(ChainValue lhs, const ChainValue& rhs) {
        return lhs *= rhs;
    }

    template <size_t I>
    auto& Get() {
        return v.template Get<I>();
    }

    template <size_t I>
    const auto& Get() const {
        return v.template Get<I>();
    }

    static ChainValue Exp(ChainValue x, const ChainValue& y) {
        if (::Get<MOD_COUNT - 1>(y)) {
            ExpApplierBig(x, y, std::make_index_sequence<MOD_COUNT>());
        } else {
            ExpApplierSmall(x, ::Get<MOD_COUNT - 1>(y).v, std::make_index_sequence<MOD_COUNT>());
        }
        return x;
    }

    Modular<MOD> Mint() const {
        return ::Get<0>(v);
    }

    friend std::istream& operator>>(std::istream& is, ChainValue& x) {
        int64_t k;
        is >> k;
        x = ChainValue{k};
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, ChainValue x) {
        return os << x.Mint();
    }

    std::strong_ordering operator<=>(const ChainValue&) const = default;
    bool operator==(const ChainValue&) const = default;

  private:
    template <size_t I>
    static void ExpHelperSmall(ChainValue& x, int y) {
        ::Get<I>(x) = std::decay_t<decltype(::Get<I>(x))>::Exp(::Get<I>(x), y);
    }

    template <size_t I>
    static void ExpHelperBig(ChainValue& x, const ChainValue& y) {
        constexpr size_t NxtI = (I >= MOD_COUNT - 2 ? I : I + 1);
        ExpHelperSmall<I>(x, ::Get<NxtI>(y).v + std::get<NxtI>(MODS));
    }

    template <size_t... Is>
    static void ExpApplierSmall(ChainValue& x, int y, std::index_sequence<Is...>) {
        (ExpHelperSmall<Is>(x, y), ...);
    }

    template <size_t... Is>
    static void ExpApplierBig(ChainValue& x, const ChainValue& y, std::index_sequence<Is...>) {
        (ExpHelperBig<Is>(x, y), ...);
    }
};

template <int MOD>
struct std::hash<ChainValue<MOD>> {
    size_t operator()(const ChainValue<MOD>& h) const {
        return h.Int();
    }
};

// const size_t MOD = 998'244'353;
// using Chain = ChainValue<MOD>;

#endif  // CODE_FOR_REUSE_MODULAR_ARITHMETICS_TOTIENT_CHAIN_HPP_