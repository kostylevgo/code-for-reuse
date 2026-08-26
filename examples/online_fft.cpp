// MIPT summer intensive 2026, contest 4, problem B

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
#include "modular_arithmetics/online_fft.hpp"
#include "modular_arithmetics/crt.hpp"

const int MOD1 = 998244353;
const int MOD2 = 167772161;

using mint1 = Modular<MOD1>;
using mint2 = Modular<MOD2>;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    cin >> a;
    OnlineFFT<mint1> f;
    OnlineFFT<mint2> g;
    ll ans = 0;
    for (auto x : a) {
        f.AddQueryA(x, 1);
        g.AddQueryA(x, 1);
    }
    for (int i = 0; i < n; ++i) {
        f.AddQueryA(a[i], -1);
        g.AddQueryA(a[i], -1);
        ans += crt(f.At(2 * a[i]), g.At(2 * a[i]));
        f.AddQueryB(a[i], 1);
        g.AddQueryB(a[i], 1);
    }
    cout << ans << endl;
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
