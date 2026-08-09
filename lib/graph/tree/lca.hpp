#ifndef CODE_FOR_REUSE_GRAPH_TREE_LCA_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_LCA_HPP_

#include "height.hpp"
#include "hld_parent.hpp"

inline int LCA(int u, int v, const Tree& t, const HLDParent& hld, const Height& h) {
    while (hld[u] != hld[v]) {
        if (h[hld[u]] > h[hld[v]]) {
            std::swap(u, v);
        }
        u = t.p[hld[u]];
    }
    if (t.tin[u] < t.tin[v]) {
        return u;
    }
    return v;
}

#endif  // CODE_FOR_REUSE_GRAPH_TREE_LCA_HPP_