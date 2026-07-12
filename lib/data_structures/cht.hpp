#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_CHT_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_CHT_HPP_

#include <algorithm>
#include <compare>
#include <functional>
#include <ranges>
#include <vector>

template <typename T>
struct LinearFunction {
    T k;
    T b;

    T operator()(T x) const {
        return k * x + b;
    }

    LinearFunction& operator+=(LinearFunction other) {
        k += other.k;
        b += other.b;
        return *this;
    }

    LinearFunction& operator-=(LinearFunction other) {
        k -= other.k;
        b -= other.b;
        return *this;
    }

    friend LinearFunction operator+(LinearFunction a, LinearFunction b) {
        a += b;
        return a;
    }

    friend LinearFunction operator-(LinearFunction a, LinearFunction b) {
        a -= b;
        return a;
    }

    bool operator==(const LinearFunction&) const = default;
    std::strong_ordering operator<=>(const LinearFunction&) const = default;
};

template <typename>
struct ReverseComparatorHelper {};

template <typename T>
struct ReverseComparatorHelper<std::less<T>> {
    using ReverseT = std::greater<T>;
};

template <typename T>
struct ReverseComparatorHelper<std::greater<T>> {
    using ReverseT = std::less<T>;
};

template <std::random_access_iterator I, std::sentinel_for<I> S, class Comp>
constexpr I PushCHT(I first, S last_s, Comp better = {}) {
    auto n = std::ranges::distance(first, last_s);

    if (n <= 1) {
        return first + n;
    }

    I inserted = first + (n - 1);
    I last = inserted - 1;
    auto inserted_function = *inserted;

    if (last->k == inserted->k) {
        if (std::invoke(better, inserted->b, last->b)) {
            *last = inserted_function;
        }
        return last + 1;
    }

    while (last != first) {
        I before_last = last - 1;
        auto l0 = *before_last;
        auto l1 = *last;
        auto l2 = inserted_function;
        l2 -= l0;
        l1 -= l0;
        if (l2.b * l1.k < l1.b * l2.k) {
            break;
        }
        last = before_last;
    }

    *(last + 1) = inserted_function;
    return last + 2;
}

template <std::ranges::random_access_range R, class Comp>
constexpr std::ranges::borrowed_iterator_t<R> PushCHT(R&& r, Comp comp = {}) {
    return PushCHT(std::ranges::begin(r), std::ranges::end(r), std::move(comp));
}

template <std::random_access_iterator I, std::sentinel_for<I> S, class Comp, typename T>
constexpr T GetCHT(I first, S last_s, T at, Comp better = {}) {
    auto n = std::ranges::distance(first, last_s);

    I left = first;
    I right = first + (n - 1);

    while (right - left > 2) {
        auto distance = std::ranges::distance(left, right);
        I l1 = left + distance / 3;
        I r1 = left + distance * 2 / 3;
        auto value_of_l1 = (*l1)(at);
        auto value_of_r1 = (*r1)(at);
        if (std::invoke(better, value_of_l1, value_of_r1)) {
            right = r1;
        } else {
            left = l1;
        }
    }
    return std::ranges::min(
        std::ranges::subrange(left, right + 1) |
            std::views::transform([at = std::move(at)](const auto& fn) { return std::invoke(fn, at); }),
        better);
}

template <std::ranges::random_access_range R, class Comp, typename T>
constexpr T GetCHT(R&& r, T at, Comp comp = {}) {
    return GetCHT(std::ranges::begin(r), std::ranges::end(r), std::move(at), std::move(comp));
}

template <class Better, typename T>
class ConvexHullTrick : private std::vector<LinearFunction<T>> {
  public:
    using Base = std::vector<LinearFunction<T>>;

    using Base::clear;
    using Base::reserve;
    using Base::size;

    using Base::begin;
    using Base::end;

    const LinearFunction<T>& operator[](size_t index) const {
        return Base::operator[](index);
    }

    void Add(LinearFunction<T> fn) {
        Base::push_back(fn);
        auto it = PushCHT(begin(), end(), Better{});
        Base::erase(it, end());
    }

    T operator()(T at) const {
        return GetCHT(*this, std::move(at), Better{});
    }

  private:
    auto begin() {
        return Base::begin();
    }

    auto end() {
        return Base::end();
    }
};

template <class Better, typename T>
class AddIterator {
  public:
    using iterator_category = std::output_iterator_tag;
    using difference_type = std::ptrdiff_t;

    explicit AddIterator(ConvexHullTrick<Better, T>& to) : to_(&to) {
    }

    AddIterator& operator++() {
        to_->Add(added_);
        return *this;
    }

    AddIterator& operator++(int) {
        return operator++();
    }

    LinearFunction<T>& operator*() {
        return added_;
    }

    LinearFunction<T>* operator->() {
        return &added_;
    }

  private:
    LinearFunction<T> added_{};
    ConvexHullTrick<Better, T>* to_ = nullptr;
};

template <class Comp, typename T>
void MergeCHT(const ConvexHullTrick<Comp, T>& a, const ConvexHullTrick<Comp, T>& b, ConvexHullTrick<Comp, T>& result) {
    result.clear();
    result.reserve(a.size() + b.size());
    std::ranges::merge(a, b, AddIterator(result), typename ReverseComparatorHelper<Comp>::ReverseT{});
}

template <typename T>
using MinCHT = ConvexHullTrick<std::less<>, T>;

template <typename T>
using MaxCHT = ConvexHullTrick<std::greater<>, T>;

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_CHT_HPP_