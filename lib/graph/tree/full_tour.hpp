#ifndef CODE_FOR_REUSE_GRAPH_TREE_FULL_TOUR_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_FULL_TOUR_HPP_

#include "tin_tout_tour.hpp"

// first entry is not flipped, others are
struct FullTour : std::vector<int> {
    FullTour() = default;

    explicit FullTour(const Tree& t, const TinToutTour& eul) {
        if (eul.empty()) {
            return;
        }
        reserve(eul.size() - 1);
        vector<int> stack;
        for (auto v : std::ranges::subrange(eul.begin(), eul.end() - 1)) {
            if (IsFlipped(v)) {
                stack.pop_back();
                push_back(Flip(stack.back()));
            } else {
                stack.push_back(v);
                push_back(v);
            }
        }
    }
};

#endif  // CODE_FOR_REUSE_GRAPH_TREE_FULL_TOUR_HPP_