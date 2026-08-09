#ifndef CODE_FOR_REUSE_GRAPH_TREE_TIN_TOUT_TOUR_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_TIN_TOUT_TOUR_HPP_

#include "tree.hpp"
#include "utils/flip.hpp"
#include "utils/radix_sort.hpp"

struct TourEntry {
    int time_key;
    int type_key;
    int v;

    std::strong_ordering operator<=>(const TourEntry&) const = default;
    bool operator==(const TourEntry&) const = default;
};

inline TourEntry TinEntry(int v, const Tree& t) {
    return {t.tin[v], (int)t.size(), v};
}

inline TourEntry ToutEntry(int v, const Tree& t) {
    return {t.tout(v), t.sub[v] - 1, Flip(v)};
}

std::vector<TourEntry> CreateEntries(const auto& r, const Tree& t) {
    std::vector<TourEntry> entries;
    entries.reserve(2 * std::ranges::size(r));
    for (auto v : r) {
        entries.emplace_back(TinEntry(v, t));
        entries.emplace_back(ToutEntry(v, t));
    }
    return entries;
}

// tins are not flipped, touts are
struct TinToutTour : std::vector<int> {
    using vector::vector;

    explicit TinToutTour(const Tree& t) : vector(2 * t.size()) {
        auto entries = CreateEntries(std::views::iota(0, (int)t.size()), t);

        std::vector<int> buf(t.size() + 1);
        std::vector<TourEntry> temp(2 * t.size());
        RadixSort(entries, temp.begin(), buf.begin(), &TourEntry::time_key);
        buf.assign(t.size() + 1, 0);
        RadixSort(temp, entries.begin(), buf.begin(), &TourEntry::type_key);

        Fill(entries);
    }

    explicit TinToutTour(const auto& v, const Tree& t) : vector(2 * std::ranges::size(v)) {
        auto entries = CreateEntries(v, t);
        std::ranges::sort(entries);
        Fill(entries);
    }

  private:
    void Fill(const std::vector<TourEntry>& entries) {
        for (int i = 0; i < size(); ++i) {
            (*this)[i] = entries[i].v;
        }
    }
};

#endif  // CODE_FOR_REUSE_GRAPH_TREE_TIN_TOUT_TOUR_HPP_