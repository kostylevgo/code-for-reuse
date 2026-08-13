#ifndef CODE_FOR_REUSE_GRAPH_TREE_VIRTUAL_TREE_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_VIRTUAL_TREE_HPP_

#include "data_structures/compressor.hpp"
#include "full_tour.hpp"
#include "lca.hpp"

struct VirtualTree {
    Tree tree;
    Compressor<int> mapping{};

    VirtualTree() = default;

    explicit VirtualTree(std::vector<int> v, const Tree& t, const HLDParent& hld, const Height& h) {
        if (v.empty()) {
            return;
        }
        std::ranges::sort(v, {}, AsFunction(t.tin));
        int cnt = v.size();
        v.resize(2 * v.size() - 1);
        for (int i = cnt; i--;) {
            v[2 * i] = v[i];
        }
        for (int i = 1; i < v.size(); i += 2) {
            v[i] = LCA(v[i - 1], v[i + 1], t, hld, h);
        }
        mapping = Compressor(v);
        tree = Tree(mapping.size());
        FullTour tour(t, TinToutTour(mapping, t));
        for (int i = 0; i + 1 < tour.size(); ++i) {
            if (!IsFlipped(tour[i + 1])) {
                tree.AddEdge(mapping(NoFlips(tour[i])), mapping(tour[i + 1]));
            }
        }
        tree.Build();
    }
};

#endif  // CODE_FOR_REUSE_GRAPH_TREE_VIRTUAL_TREE_HPP_