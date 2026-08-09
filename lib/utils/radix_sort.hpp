#ifndef CODE_FOR_REUSE_UTILS_RADIX_SORT_HPP_
#define CODE_FOR_REUSE_UTILS_RADIX_SORT_HPP_

#include <algorithm>
#include <functional>
#include <numeric>
#include <ranges>

template <std::random_access_iterator I, std::sentinel_for<I> S, std::random_access_iterator Buf,
          class Proj = std::identity>
constexpr I RadixAccount(I it, S last_s, Buf buf, Proj proj = {}) {
    for (; it != last_s; ++it) {
        int index = std::invoke(proj, *it);
        *std::next(buf, index) += 1;
    }
    return it;
}

template <std::random_access_iterator Buf>
constexpr void RadixSumBuffer(Buf buf, int size) {
    std::exclusive_scan(buf, buf + size, buf, 0);
}

template <std::random_access_iterator I, std::sentinel_for<I> S, std::random_access_iterator Out,
          std::random_access_iterator Buf, class Proj = std::identity>
constexpr I RadixMove(I it, S last_s, Out out, Buf buf, Proj proj = {}) {
    for (; it != last_s; ++it) {
        int index = std::invoke(proj, *it);
        int pos = (*std::next(buf, index))++;
        *std::next(out, pos) = *it;
    }
    return it;
}

template <std::random_access_iterator I, std::sentinel_for<I> S, std::random_access_iterator Out,
          std::random_access_iterator Buf, class Proj = std::identity>
constexpr I RadixSort(I first, S last_s, Out out, Buf buf, Proj proj = {}) {
    RadixAccount(first, last_s, buf, proj);
    RadixSumBuffer(buf, std::invoke(proj, *std::ranges::max_element(first, last_s, {}, proj)) + 1);
    return RadixMove(first, last_s, out, buf, proj);
}

template <std::ranges::random_access_range R, std::random_access_iterator Out, std::random_access_iterator Buf,
          class Proj = std::identity>
constexpr std::ranges::borrowed_iterator_t<R> RadixAccount(R&& r, Buf buf, Proj proj = {}) {
    return RadixAccount(std::ranges::begin(r), std::ranges::end(r), buf, std::move(proj));
}

template <std::ranges::random_access_range R, std::random_access_iterator Out, std::random_access_iterator Buf,
          class Proj = std::identity>
constexpr std::ranges::borrowed_iterator_t<R> RadixSumBuffer(R&& r, Proj proj = {}) {
    return RadixSumBuffer(std::ranges::begin(r), std::ranges::size(r), std::move(proj));
}

template <std::ranges::random_access_range R, std::random_access_iterator Out, std::random_access_iterator Buf,
          class Proj = std::identity>
constexpr std::ranges::borrowed_iterator_t<R> RadixMove(R&& r, Out out, Buf buf, Proj proj = {}) {
    return RadixSort(std::ranges::begin(r), std::ranges::end(r), out, buf, std::move(proj));
}

template <std::ranges::random_access_range R, std::random_access_iterator Out, std::random_access_iterator Buf,
          class Proj = std::identity>
constexpr std::ranges::borrowed_iterator_t<R> RadixSort(R&& r, Out out, Buf buf, Proj proj = {}) {
    return RadixSort(std::ranges::begin(r), std::ranges::end(r), out, buf, std::move(proj));
}

#endif  // CODE_FOR_REUSE_UTILS_RADIX_SORT_HPP_