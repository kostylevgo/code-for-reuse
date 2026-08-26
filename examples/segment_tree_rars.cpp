// https://judge.yosupo.jp/problem/range_affine_range_sum

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

#include "utils/container_as_function.hpp"
#include "data_structures/monoid/top_down_segment_tree.hpp"
#include "data_structures/monoid/bottom_up_segment_tree.hpp"
#include "io/vector_io.hpp"
#include "modular_arithmetics/modular.hpp"

using mint = Modular<998244353>;

struct Affine {
    mint b = 1;
    mint c = 0;

    void Push(Affine& down) const {
        down.c *= b;
        down.c += c;
        down.b *= b;
    }

    void Push(mint& s, auto iter) const {
        s *= b;
        s += c * iter.Length();
    }
};

struct AffineScheme {
    mint s;
    Affine f;

    void Push(AffineScheme& down, auto) const {
        f.Push(down.f);
    }

    void Push(auto iter) {
        f.Push(s, iter);
        f = Affine{};
    }

    friend AffineScheme operator+(AffineScheme a, AffineScheme b) {
        return {a.s + b.s, {}};
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<mint> a(n);
    cin >> a;
    // TopDownSegmentTree<AffineScheme>
    BottomUpSegmentTree<AffineScheme>
    st(n, AsFunction(a));
    while (m--) {
        int t;
        cin >> t;
        if (t == 0) {
            int l, r;
            mint b, c;
            cin >> l >> r >> b >> c;
            AffineScheme push{{}, Affine{b, c}};
            st.Push(l, r, push);
        } else {
            int l, r;
            cin >> l >> r;
            cout << st.Get(l, r).s << endl;
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
