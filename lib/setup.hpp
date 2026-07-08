#pragma once

#include "bits.hpp"

#define DETAIL_SOLVE_COMMON(args)\
using namespace std;\
void solve(){\
    args\
}\
int32_t main() {\
    setIO();\
    Stopwatch s;

#define SOLVE_SINGLETEST(args) DETAIL_SOLVE_COMMON(args)\
    solve();\
}

#define SOLVE_MULTITEST(args) DETAIL_SOLVE_COMMON(args)\
    def(int, t);\
    while (t--) solve();\
}