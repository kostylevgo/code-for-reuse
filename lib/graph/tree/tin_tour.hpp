#ifndef CODE_FOR_REUSE_GRAPH_TREE_TIN_TOUR_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_TIN_TOUR_HPP_

#include "tree.hpp"
#include "utils/permutations.hpp"

struct TinTour : std::vector<int> {
    using vector::vector;

    explicit TinTour(const Tree& t) : vector(Inverse(t.tin)) {
    }
};

#endif  // CODE_FOR_REUSE_GRAPH_TREE_TIN_TOUR_HPP_