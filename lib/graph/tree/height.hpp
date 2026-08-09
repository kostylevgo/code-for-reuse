#ifndef CODE_FOR_REUSE_GRAPH_TREE_HEIGHT_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_HEIGHT_HPP_

#include "tin_tour.hpp"

struct Height : std::vector<int> {
    using vector::vector;

    explicit Height(const Tree& t, const TinTour& eul) : vector(t.size()) {
        for (auto v : eul) {
            for (auto u : t[v]) {
                (*this)[u] = (*this)[v] + 1;
            }
        }
    }
};

#endif  // CODE_FOR_REUSE_GRAPH_TREE_HEIGHT_HPP_