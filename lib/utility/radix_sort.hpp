#ifndef CODE_FOR_REUSE_UTILITY_RADIX_SORT_HPP_
#define CODE_FOR_REUSE_UTILITY_RADIX_SORT_HPP_

#include <algorithm>
#include <functional>
#include <ranges>

template <std::random_access_iterator I, std::sentinel_for<I> S, std::random_access_iterator Out,
          std::random_access_iterator Buf, class Proj = std::identity>
constexpr I RadixSort(I first, S last_s, Out out, Buf buf, Proj proj = {}) {
    int max_index = 0;

    for (auto it = first; it != last_s; ++it) {
        int index = std::invoke(proj, *it);
        max_index = std::max(max_index, index);
        *std::next(buf, index) += 1;
    }

    int sum = 0;
    auto it = buf;
    for (int i = 0; i <= max_index; ++i) {
        int add = *it;
        *it = sum;
        sum += add;
        ++it;
    }

    for (auto it = first; it != last_s; ++it) {
        int index = std::invoke(proj, *it);
        int pos = (*std::next(buf, index))++;
        *std::next(out, pos) = *it;
    }

    auto n = std::ranges::distance(first, last_s);
    return std::next(first, n);
}

template <std::ranges::random_access_range R, std::random_access_iterator Out, std::random_access_iterator Buf,
          class Proj = std::identity>
constexpr std::ranges::borrowed_iterator_t<R> RadixSort(R&& r, Out out, Buf buf, Proj proj = {}) {
    return RadixSort(std::ranges::begin(r), std::ranges::end(r), out, buf, std::move(proj));
}

#endif  // CODE_FOR_REUSE_UTILITY_RADIX_SORT_HPP_