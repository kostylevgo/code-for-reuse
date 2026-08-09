#include <vector>

struct UsedCounter {
    explicit UsedCounter(int n = 0): used(n) {}

    void Assign(int n) {
        used.assign(n, 0);
        t = 1;
    }

    bool operator[](int i) const {
        return used[i] >= t;
    }

    int Get(int i) const {
        return used[i];
    }

    void Set(int i, int val) {
        used[i] = val;
    }

    void Use(int i) {
        used[i] = t;
    }

    void Reset() {
        ++t;
    }

    std::vector<int> used;
    int t = 1;
};