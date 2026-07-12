#ifndef CODE_FOR_REUSE_IO_IO_HPP_
#define CODE_FOR_REUSE_IO_IO_HPP_

#include <array>
#include <iostream>
#include <ranges>
#include <tuple>
#include <utility>

inline namespace IO {

template <class T>
concept DefaultI = requires(T a, std::basic_istream<char>& is) { is >> a; };
template <class T>
concept DefaultO = requires(T a, std::basic_ostream<char>& os) { os << a; };
template <class T>
concept TupleLike = requires(T a, size_t n) { std::tuple_size_v<T>; };

template <auto& is>
struct Reader {
    template <class T>
    void Impl(T& t) {
        if constexpr (DefaultI<T>)
            is >> t;
        else if constexpr (std::ranges::range<T>) {
            for (auto& x : t) Impl(x);
        } else if constexpr (TupleLike<T>) {
            std::apply([this](auto&... args) { (Impl(args), ...); }, t);
        } else
            static_assert(TupleLike<T>, "No matching type for read");
    }

    template <class... Ts>
    void read(Ts&... ts) {
        ((Impl(ts)), ...);
    }
};

template <class... Ts>
void re(Ts&... ts) {
    Reader<std::cin>{}.read(ts...);
}

#define def(t, args...) \
    t args;             \
    re(args);

#define de(t, name, args...) \
    t name(args);            \
    re(name);

template <auto& os, bool debug, bool print_nd>
struct Writer {
    static constexpr std::string comma = debug ? "," : "";

    template <class T>
    constexpr char Space(const T&) const {
        return print_nd && (std::ranges::range<T> or TupleLike<T>) ? '\n' : ' ';
    }

    template <class T>
    void Impl(T const& t) const {
        if constexpr (DefaultO<T>)
            os << t;
        else if constexpr (std::ranges::range<T>) {
            if (debug) os << '{';
            int i = 0;
            for (auto&& x : t) ((i++) ? (os << comma << Space(x), Impl(x)) : Impl(x));
            if (debug) os << '}';
        } else if constexpr (TupleLike<T>) {
            if (debug) os << '(';
            std::apply(
                [this](auto const&... args) {
                    int i = 0;
                    (((i++) ? (os << comma << " ", Impl(args)) : Impl(args)), ...);
                },
                t);
            if (debug) os << ')';
        } else
            static_assert(TupleLike<T>, "No matching type for print");
    }

    template <class T>
    void ImplWrapper(T const& t) const {
        if (debug) os << "\033[0;31m";
        Impl(t);
        if (debug) os << "\033[0m";
    }

    template <class... Ts>
    void print(Ts const&... ts) const {
        ((Impl(ts)), ...);
    }

    template <class F, class... Ts>
    void print_with_sep(const std::string& sep, F const& f, Ts const&... ts) const {
        ImplWrapper(f), ((os << sep, ImplWrapper(ts)), ...), os << '\n';
    }

    void print_with_sep(const std::string&) const {
        os << '\n';
    }
};

template <class... Ts>
void pr(Ts const&... ts) {
    Writer<std::cout, false, true>{}.print(ts...);
}

template <class... Ts>
void ps(Ts const&... ts) {
    Writer<std::cout, false, true>{}.print_with_sep(" ", ts...);
}

}  // namespace IO

#endif  // CODE_FOR_REUSE_IO_IO_HPP_