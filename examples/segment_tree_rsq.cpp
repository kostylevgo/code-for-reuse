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
#include "data_structures/scheme/top_down_segment_tree.hpp"
#include "functional/container_as_function.hpp"

struct SumMonoid : NoPush<SumMonoid> {
    SumMonoid() = default;

    explicit SumMonoid(ll x): val(x) {}

    static SumMonoid Combine(const SumMonoid& a, const SumMonoid& b) {
        return SumMonoid{a.val + b.val};
    }

    ll val = 0;
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    cin >> a;
    TopDownSegmentTree<SumMonoid> d(n, AsFunction(a));
    while (m--) {
        int t, x, y;
        cin >> t >> x >> y;
        --x;
        if (t == 0) {
            // cout << d.Get(x, y).val << endl;
            cout << d.Get(Include(x), Exclude(y)).val << endl;
        } else {
            d.UpdateAt(x, Setter{y});
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
