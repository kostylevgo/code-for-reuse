#ifndef MULTIHASH_HPP_
#define MULTIHASH_HPP_

#include "../modular_arithmetics/modular.hpp"

template <int...>
struct HashValue {};

template <>
struct HashValue<> {
    HashValue() = default;
    HashValue(int) {}
    bool operator==(const HashValue&) const { return true; }
    strong_ordering operator<=>(const HashValue&) const { return strong_ordering::equal; }
    HashValue& operator*=(const HashValue&) { return *this; }
    HashValue& operator+=(const HashValue&) { return *this; }
    HashValue& operator-=(const HashValue&) { return *this; }
    HashValue operator*(const HashValue&) const { return *this; }
    HashValue operator+(const HashValue&) const { return *this; }
    HashValue operator-(const HashValue&) const { return *this; }
};

template <int MOD0, int... MODS>
struct HashValue<MOD0, MODS...> {
    HashValue() = default;
    HashValue(int x) : a(x), others(x) {}
    bool operator==(const HashValue&) const = default;
    strong_ordering operator<=>(const HashValue&) const = default;
    HashValue& operator*=(const HashValue& other) {
        a *= other.a;
        others *= other.others;
        return *this;
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
    HashValue operator*(const HashValue& other) const {
        HashValue copy = *this;
        copy *= other;
        return copy;
    }
    HashValue operator+(const HashValue& other) const {
        HashValue copy = *this;
        copy += other;
        return copy;
    }
    HashValue operator-(const HashValue& other) const {
        HashValue copy = *this;
        copy -= other;
        return copy;
    }

    Modular<MOD0> a;
    HashValue<MODS...> others;
};

namespace std {

template <int... MODS>
struct hash<HashValue<MODS...>> {
    size_t operator()(const HashValue<MODS...>& h) const { return h.a.v; }
};

}  // namespace std

using Hash = HashValue<1'000'000'007, 1'000'000'009>;

#endif