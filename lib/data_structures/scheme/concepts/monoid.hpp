#ifndef MONOID_HPP_
#define MONOID_HPP_
#include <concepts>
#include <tuple>

template <typename T>
concept Monoid = std::default_initializable<T> && requires(const T& a, const T& b) { { T::Combine(a, b) } -> std::same_as<T>; };

template <typename T, typename U>
concept MonoidOver = Monoid<T> && std::constructible_from<U>;

template <typename... Ms>
struct MonoidProduct : public std::tuple<Ms...> {
    static MonoidProduct Combine(const MonoidProduct& a, const MonoidProduct& b) {
        return CombineHelper(a, b, std::make_index_sequence<std::tuple_size_v<MonoidProduct>>());
    }

private:
    template <size_t... I>
    static MonoidProduct CombineHelper(const MonoidProduct& a, const MonoidProduct& b, std::index_sequence<I...>) {
        return std::make_tuple((Combine(std::get<I>(a), std::get<I>(b)), ...));
    }
};


#endif
