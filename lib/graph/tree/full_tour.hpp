#ifndef CODE_FOR_REUSE_GRAPH_TREE_FULL_TOUR_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_FULL_TOUR_HPP_

#include "tin_tout_tour.hpp"

// first entry is not flipped, others are
struct FullTour : std::vector<int> {
    using vector::vector;

    explicit FullTour(const Tree& t, const TinToutTour& eul) {
        if (eul.empty()) {
            return;
        }
        reserve(eul.size() - 1);
        for (auto v : std::ranges::subrange(eul.begin(), eul.end() - 1)) {
            if (IsFlipped(v)) {
                push_back(Flip(t.p[Flip(v)]));
            } else {
                push_back(v);
            }
        }
    }
};

#endif  // CODE_FOR_REUSE_GRAPH_TREE_FULL_TOUR_HPP_