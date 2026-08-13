#ifndef CODE_FOR_REUSE_GRAPH_TREE_TREE_HPP_
#define CODE_FOR_REUSE_GRAPH_TREE_TREE_HPP_

#include <algorithm>
#include <ranges>
#include <vector>

#include "utils/container_as_function.hpp"

struct Tree : std::vector<std::vector<int>> {
    using Base = vector;
    static constexpr int Root = 0;

    std::vector<int> p{};
    std::vector<int> sub{};
    std::vector<int> tin{};

    Tree() = default;

    explicit Tree(int n) : Base(n), p(n), sub(n), tin(n) {
    }

    void AddEdge(int u, int v) {
        (*this)[u].push_back(v);
        (*this)[v].push_back(u);
    }

    int tout(int v) const {
        return tin[v] + sub[v];
    }

    void Build() {
        int t = 0;
        DFS(0, -1);
        DFS2(0, t);
    }

    bool IsChild(int ancestor, int child) const {
        return tin[ancestor] <= tin[child] && tout(child) <= tout(ancestor);
    }

    int WhoIsFather(int a, int b) const {  // NOLINT
        if (p[a] == b) {
            return b;
        }
        if (p[b] == a) {
            return a;
        }
        exit(1);
    }

    int WhoIsSon(int a, int b) const {
        return WhoIsFather(a, b) ^ a ^ b;
    }

  private:
    void DFS(int v, int r) {  // NOLINT
        p[v] = r;
        sub[v] = 1;
        if (r != -1) {
            (*this)[v].erase(std::ranges::find((*this)[v], r));
        }
        for (auto u : (*this)[v]) {
            DFS(u, v);
            sub[v] += sub[u];
        }
        if (!(*this)[v].empty()) {
            int& max_u = *std::ranges::max_element((*this)[v], {}, AsFunction(sub));
            std::swap((*this)[v][0], max_u);
        }
    }

    void DFS2(int v, int& t) {
        tin[v] = t++;
        for (auto u : (*this)[v]) {
            DFS2(u, t);
        }
    }
};

#endif  // CODE_FOR_REUSE_GRAPH_TREE_TREE_HPP_