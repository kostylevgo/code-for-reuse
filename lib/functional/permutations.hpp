#include <vector>

std::vector<int> Inverse(const std::vector<int>& p) {
    std::vector<int> ans(p.size());
    for (int i = 0; i < p.size(); ++i) {
        ans[p[i]] = i;
    }
    return ans;
}