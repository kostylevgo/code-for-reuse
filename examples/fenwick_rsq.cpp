// https://codeforces.com/gym/100246, problem A

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

void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    cin >> a;
    StaticFenwick<ll, 100'000> d{};
    for (int i = 0; i < n; ++i) {
        d.Add(i, a[i]);
    }
    while (m--) {
        int t, x, y;
        cin >> t >> x >> y;
        --x;
        if (t == 0) {
            cout << d.Get(y - 1) - d.Get(x - 1) << endl;
        } else {
            int diff = y - a[x];
            a[x] = y;
            d.Add(x, diff);
        }
    }
}

int32_t main() {
#ifdef LOCAL
    freopen("../in.txt", "r", stdin);
    freopen("../out.txt", "w", stdout);
    const auto start = clock();
#else
    freopen("./rsq.in", "r", stdin);
    freopen("./rsq.out", "w", stdout);
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
