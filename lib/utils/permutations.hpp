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

struct CyclePartition {
    std::vector<std::vector<int>> cycle_list;
    std::vector<int> cycle_id;
    std::vector<int> cycle_pos;
};

CyclePartition Cycles(const std::vector<int>& p) {
    CyclePartition res;
    res.cycle_id.assign(p.size(), -1);
    res.cycle_pos.assign(p.size(), -1);
    for (int i = 0; i < p.size(); ++i) {
        if (res.cycle_id[i] != -1) {
            continue;
        }
        int id = res.cycle_list.size();
        res.cycle_list.emplace_back();
        int v = i;
        do {
            res.cycle_id[v] = id;
            res.cycle_pos[v] = res.cycle_list.back().size();
            res.cycle_list.back().push_back(v);
            v = p[v];
        } while (v != i);
    }
    return res;
}

#endif  // CODE_FOR_REUSE_UTILS_PERMUTATIONS_HPP_