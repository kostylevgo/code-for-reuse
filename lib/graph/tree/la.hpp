#ifndef CODE_FOR_REUSE_GRAPH_TREE_LA_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_LA_HPP_

#include "height.hpp"
#include "hld_parent.hpp"

inline int LevelAncestor(int v, int upward, const Tree& t, const TinTour& eul, const HLDParent& hld, const Height& h) {
    while (true) {
        int top = hld[v];
        int max_delta = h[v] - h[top];
        if (upward <= max_delta) {
            return eul[t.tin[v] - upward];
        }
        upward -= max_delta + 1;
        v = t.p[top];
    }
}

#endif  // CODE_FOR_REUSE_GRAPH_TREE_LA_HPP_