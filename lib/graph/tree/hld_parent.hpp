#ifndef CODE_FOR_REUSE_GRAPH_TREE_HLD_PARENT_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_HLD_PARENT_HPP_

#include <numeric>

#include "tin_tour.hpp"

struct HLDParent : std::vector<int> {
    HLDParent() = default;

    explicit HLDParent(const Tree& t, const TinTour& eul) : vector(t.size()) {
        std::iota(begin(), end(), 0);
        if (t.empty()) {
            return;
        }
        for (auto v : eul) {
            if (!t[v].empty()) {
                (*this)[t[v][0]] = (*this)[v];
            }
        }
    }
};

#endif  // CODE_FOR_REUSE_GRAPH_TREE_HLD_PARENT_HPP_