//https://judge.yosupo.jp/problem/longest_increasing_subsequence

#include <bits/stdc++.h>
#ifndef LOCAL
#define endl '\n'
#endif
// #define int int64_t

using namespace std;

typedef int64_t ll;
typedef long double ld;

constexpr int INF = 1'000'000'009;
constexpr ll INF18 = 1'000'000'000'000'000'018;

#include "io/vector_io.hpp"
#include "data_structures/fenwick.hpp"
#include "data_structures/compressor.hpp"

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    cin >> a;
    Compressor c(a);

    vector<int> ans(n, 0);
    vector<int> parent(n, -1);
    Fenwick<pair<int, int>> f(c.size(), pair{0, -1});

    for (int i = 0; i < n; ++i) {
        int pos = c(a[i]);
        auto [len, par] = f.Get(pos - 1, MaxAggregator(pair{0, -1})).mx;
        ++len;
        ans[i] = len;
        parent[i] = par;
        f.Add(pos, MaxUpdater(pair{len, i}));
    }

    auto i = ranges::max_element(ans) - ans.begin();
    vector<int> seq;
    while (i != -1) {
        seq.push_back(i);
        i = parent[i];
    }
    ranges::reverse(seq);
    cout << seq.size() << endl << seq;
}

int32_t main() {
#ifdef LOCAL
    freopen("../in.txt", "r", stdin);
    freopen("../out.txt", "w", stdout);
    const auto start = clock();
#else
    cin.tie(0)->sync_with_stdio(false);
#endif
    int ttt = 1;
    // cin >> ttt;
    while (ttt-- > 0) solve();
#ifdef LOCAL
    cerr << setprecision(3) << fixed << endl
         << "TIME: " << static_cast<double>(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
#endif
}
