#ifndef FENWICK_H_
#define FENWICK_H_

#include <vector>
#include <array>
#include <ranges>

inline void Down(int& pos) {
    pos &= pos + 1;
    --pos;
}

inline void Up(int& pos) {
    pos |= pos + 1;
}

template <std::random_access_iterator I, auto pos_transform>
class IteratorAdapter : public std::iterator_traits<I> {
public:
    using iterator_category = std::forward_iterator_tag;

    IteratorAdapter() = default;

    IteratorAdapter(I first, int index): first_(first), pos_(index) {}

    IteratorAdapter& operator++() {
        pos_transform(pos_);
        return *this;
    }

    IteratorAdapter operator++(int) {
        IteratorAdapter cur = *this;
        ++*this;
        return cur;
    }

    explicit operator I() const {
        return first_ + pos_;
    }

    auto operator->() const {
        return I(*this);
    }

    auto& operator*() const {
        return *I(*this);
    }

    int GetPos() const {
        return pos_;
    }

    template <typename S>
    bool operator==(S sentinel) const {
        return sentinel.Check(GetPos());
    }

private:
    I first_;
    int pos_;
};

template <std::random_access_iterator I>
using FenwickPrefixIterator = IteratorAdapter<I, Down>;

struct FenwickPrefixSentinel {
    bool Check(int pos) const {
        return pos == -1;
    }
};

template <std::random_access_iterator I>
using FenwickAncestorsIterator = IteratorAdapter<I, Up>;

struct FenwickAncestorsSentinel {
    int size;

    bool Check(int pos) const {
        return pos >= size;
    }
};

template <std::random_access_iterator I, std::sentinel_for<I> S>
constexpr auto FenwickPrefix(I first, S last, int index) {
    return std::ranges::subrange(FenwickPrefixIterator(first, index), FenwickPrefixSentinel{});
}

template <std::ranges::random_access_range R>
constexpr auto FenwickPrefix(R&& r, int index) {
    return FenwickPrefix(std::ranges::begin(r), std::ranges::end(r), index);
}

template <std::random_access_iterator I, std::sentinel_for<I> S>
constexpr auto FenwickAncestors(I first, S last, int index) {
    return std::ranges::subrange(FenwickAncestorsIterator(first, index), FenwickAncestorsSentinel{static_cast<int>(std::ranges::distance(first, last))});
}

template <std::ranges::random_access_range R>
constexpr auto FenwickAncestors(R&& r, int index) {
    return FenwickAncestors(std::ranges::begin(r), std::ranges::end(r), index);
}

template <class T, class Base>
class GenericFenwick : public Base {
public:
    template <typename... Args>
    explicit GenericFenwick(Args&&... args): Base(std::forward<Args>(args)...) {}

    using Base::size;

    template <class U>
    void Add(int i, U&& u) {
        for (auto& x : FenwickAncestors(*this, i)) {
            x += u;
        }
    }

    template <typename U = T>
    auto Get(int i, U ans = T{}) const {
        for (auto& x : FenwickPrefix(*this, i)) {
            ans += x;
        }
        return ans;
    }

    T GetSegment(int l, int r) { // half-interval with r excluded
        return Get(r - 1) - Get(l - 1);
    }
};

template <class T>
using Fenwick = GenericFenwick<T, std::vector<T>>;

template <class T, size_t N>
using StaticFenwick = GenericFenwick<T, std::array<T, N>>;

// for monotonous max fenwick

template <typename T>
struct MaxAggregator {
    T mx;

    void operator+=(const T& other) {
        mx = max(other, mx);
    }
};

template <typename T>
struct MaxUpdater {
    T mx;

    friend void operator+=(T& x, const MaxUpdater& u) {
        x = max(x, u.mx);
    }
};

#endif
