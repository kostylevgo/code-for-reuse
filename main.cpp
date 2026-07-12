#include "setup.hpp"

SOLVE_MULTITEST(
    def(int, n);
    de(vector<int>, a, n);
    int i_mx = ranges::max_element(a) - a.begin();
    swap(a[0], a[i_mx]);
    ps(a);
)
