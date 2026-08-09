#ifndef CODE_FOR_REUSE_GRAPH_TREE_VIRTUAL_TREE_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_VIRTUAL_TREE_HPP_

#include "data_structures/compressor.hpp"
#include "full_tour.hpp"
#include "lca.hpp"

struct VirtualTree {
    Tree image;
    Compressor<int> mapping{};
};

inline VirtualTree GetVirtualTree(std::vector<int> v, const Tree& t, const HLDParent& hld, const Height& h) {
    if (v.empty()) {
        return {};
    }
    std::ranges::sort(v, {}, AsFunction(t.tin));
    int cnt = v.size();
    v.resize(2 * v.size() - 1);
    for (int i = 1; i < cnt; ++i) {
        v[2 * i] = v[i];
    }
    for (int i = 1; i < v.size(); i += 2) {
        v[i] = LCA(v[i - 1], v[i + 1], t, hld, h);
    }
    Compressor c(v);
    Tree ans(c.size());
    FullTour tour(t, TinToutTour(c, t));
    for (int i = 0; i + 1 < tour.size(); ++i) {
        ans.AddEdge(c(tour[i]), c(tour[i + 1]));
    }
    return {std::move(ans), std::move(c)};
}

#endif  // CODE_FOR_REUSE_GRAPH_TREE_VIRTUAL_TREE_HPP_