// https://judge.yosupo.jp/problem/point_set_range_composite

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
#include "modular_arithmetics/modular.hpp"
#include "functional/container_as_function.hpp"

struct Fun : NoPush<Fun> {
    mint a = 1;
    mint b = 0;

    Fun() = default;

    Fun(mint a, mint b): a(a), b(b) {}

    friend istream& operator>>(istream& is, Fun& f) {
        return is >> f.a >> f.b;
    }

    mint operator()(mint x) const {
        return a * x + b;
    }

    static Fun Combine(Fun left, Fun right) {
        return Fun{left.a * right.a, right(left.b)};
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<Fun> a(n);
    cin >> a;
    TopDownSegmentTree<Fun> st(n, AsFunction(a));
    while (m--) {
        int t;
        cin >> t;
        if (t == 0) {
            int p;
            Fun f;
            cin >> p >> f;
            st.UpdateAt(p, Setter{f});
        } else {
            int l, r;
            mint x;
            cin >> l >> r >> x;
            cout << st.Get(l, r)(x) << endl;
        }
    }
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
