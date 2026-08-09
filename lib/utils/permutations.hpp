#ifndef CODE_FOR_REUSE_UTILS_PERMUTATIONS_HPP_
#define CODE_FOR_REUSE_UTILS_PERMUTATIONS_HPP_

#include <vector>

std::vector<int> Inverse(const std::vector<int>& p) {
    std::vector<int> ans(p.size());
    for (int i = 0; i < p.size(); ++i) {
        ans[p[i]] = i;
    }
    return ans;
}

std::vector<std::vector<int>> Cycles(std::vector<int> p) {
    std::vector<std::vector<int>> ans;
    for (int i = 0; i < p.size(); ++i) {
        if (p[i] == -1) {
            continue;
        }
        ans.emplace_back();
        int v = i;
        do {
            ans.back().push_back(v);
            int nxt = p[v];
            p[v] = -1;
            v = nxt;
        } while (v != i);
    }
    return ans;
}

#endif  // CODE_FOR_REUSE_UTILS_PERMUTATIONS_HPP_