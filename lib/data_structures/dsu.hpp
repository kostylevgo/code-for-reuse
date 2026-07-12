#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_DSU_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_DSU_HPP_

#include <memory>
#include <vector>

class DSU {
  public:
    explicit DSU(int n) : dsu(n, -1) {
    }

    int Get(int v) {
        int ans = v;
        while (dsu[ans] >= 0) {
            ans = dsu[ans];
        }
        while (v != ans) {
            int next_v = dsu[v];
            dsu[v] = ans;
            v = next_v;
        }
        return ans;
    }

    bool Unite(int i, int j) {
        i = Get(i), j = Get(j);
        if (i == j) return false;
        if (dsu[i] > dsu[j]) std::swap(i, j);
        dsu[i] += dsu[j];
        dsu[j] = i;
        return true;
    }

    bool Check(int i, int j) {
        return Get(i) == Get(j);
    }

  private:
    std::vector<int> dsu;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_DSU_HPP_