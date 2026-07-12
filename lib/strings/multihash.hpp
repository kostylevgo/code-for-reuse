#ifndef CODE_FOR_REUSE_STRINGS_MULTIHASH_HPP_
#define CODE_FOR_REUSE_STRINGS_MULTIHASH_HPP_

#include "../modular_arithmetics/modular.hpp"

template <int...>
struct HashValue {};

template <>
struct HashValue<> {
    HashValue() = default;

    HashValue(int64_t) {
    }

    HashValue& operator+=(HashValue) {
        return *this;
    }

    HashValue& operator-=(HashValue) {
        return *this;
    }

    HashValue& operator*=(HashValue) {
        return *this;
    }

    friend HashValue operator+(HashValue, HashValue) {
        return HashValue{};
    }

    friend HashValue operator-(HashValue, HashValue) {
        return HashValue{};
    }

    friend HashValue operator*(HashValue, HashValue) {
        return HashValue{};
    }

    friend HashValue operator-(HashValue) {
        return HashValue{};
    }

    bool operator==(const HashValue&) const {
        return true;
    }

    std::strong_ordering operator<=>(const HashValue&) const {
        return std::strong_ordering::equal;
    }
};

template <int MOD0, int... MODS>
struct HashValue<MOD0, MODS...> {
    Modular<MOD0> a;
    HashValue<MODS...> others;

    HashValue() = default;

    HashValue(int64_t x) : a(x), others(x) {
    }

    HashValue& operator+=(const HashValue& other) {
        a += other.a;
        others += other.others;
        return *this;
    }

    HashValue& operator-=(const HashValue& other) {
        a -= other.a;
        others -= other.others;
        return *this;
    }

    HashValue& operator*=(const HashValue& other) {
        a *= other.a;
        others *= other.others;
        return *this;
    }

    friend HashValue operator+(HashValue lhs, const HashValue& rhs) {
        return lhs += rhs;
    }

    friend HashValue operator-(HashValue lhs, const HashValue& rhs) {
        return lhs -= rhs;
    }

    friend HashValue operator*(HashValue lhs, const HashValue& rhs) {
        return lhs *= rhs;
    }

    friend HashValue operator-(const HashValue& lhs) {
        return 0 - lhs;
    }

    template <size_t I>
    auto& Get() {
        if constexpr (I == 0) {
            return a;
        } else {
            return others.template Get<I - 1>();
        }
    }

    template <size_t I>
    const auto& Get() const {
        if constexpr (I == 0) {
            return a;
        } else {
            return others.template Get<I - 1>();
        }
    }

    friend std::istream& operator>>(std::istream& is, HashValue& v) {
        int64_t x;
        is >> x;
        v = HashValue{x};
        return is;
    }

    std::strong_ordering operator<=>(const HashValue&) const = default;
    bool operator==(const HashValue&) const = default;
};

template <size_t I>
auto&& Get(auto&& v) {
    return v.template Get<I>();
}

template <int... MODS>
struct std::hash<HashValue<MODS...>> {
    size_t operator()(const HashValue<MODS...>& h) const {
        return h.a.v;
    }
};

#endif  // CODE_FOR_REUSE_STRINGS_MULTIHASH_HPP_