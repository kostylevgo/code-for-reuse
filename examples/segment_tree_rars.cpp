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

#include "io/vector_io.hpp"
#include "data_structures/scheme/top_down_segment_tree.hpp"
#include "modular_arithmetics/modular.hpp"
#include "functional/container_as_function.hpp"

struct SumMonoid {
    mint x = 0;

    static SumMonoid Combine(SumMonoid a, SumMonoid b) {
        return SumMonoid{a.x + b.x};
    }
};

struct Affine {
    mint b = 1;
    mint c = 0;

    void Push(Affine& down) const {
        down.c *= b;
        down.c += c;
        down.b *= b;
    }

    void Push(SumMonoid& s, SegmentTreeIterator iter) const {
        s.x *= b;
        s.x += c * iter.Length();
    }
};

struct AffineScheme {
    Affine f;
    SumMonoid s;

    SumMonoid& Get() {
        return s;
    }

    const SumMonoid& Get() const {
        return s;
    }

    void Push(AffineScheme& down) const {
        f.Push(down.f);
    }

    void SelfPush(SegmentTreeIterator i) {
        f.Push(s, i);
    }

    void Reset() {
        f = Affine{};
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<mint> a(n);
    cin >> a;
    TopDownSegmentTree<AffineScheme> st(n, AsFunction(a));
    while (m--) {
        int t;
        cin >> t;
        if (t == 0) {
            int l, r;
            mint b, c;
            cin >> l >> r >> b >> c;
            AffineScheme push{Affine{b, c}, {}};
            st.Update(l, r, push);
        } else {
            int l, r;
            cin >> l >> r;
            cout << st.Get(l, r).x << endl;
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
