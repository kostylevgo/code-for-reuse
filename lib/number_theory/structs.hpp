#ifndef CODE_FOR_REUSE_NUMBER_THEORY_STRUCTS_HPP_
#define CODE_FOR_REUSE_NUMBER_THEORY_STRUCTS_HPP_

#include <compare>

template <typename T>
struct Divisor {
    T divisor = 0;
    int power = 0;

    constexpr std::strong_ordering operator<=>(const Divisor&) const = default;
    constexpr bool operator==(const Divisor&) const = default;
};

#endif  // CODE_FOR_REUSE_NUMBER_THEORY_STRUCTS_HPP_