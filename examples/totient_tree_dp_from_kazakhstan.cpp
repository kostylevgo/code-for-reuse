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

#include "modular_arithmetics/totient_chain.hpp"

const size_t MOD = 998'244'353;
using Chain = ChainValue<MOD>;
using mint = Modular<MOD>;

const int N = 100'010;

vector<int> g[N];
Chain e_minus_one[N], r_times_k[N], dp[N];
Chain k;

Chain h_times_k(Chain x) {
    return Chain::Exp(k + 1, x) - 1;
}

void dfs(int v, int r) {
    for (auto u : g[v]) {
        if (u == r) continue;
        dfs(u, v);
        e_minus_one[v] += Chain::Exp(k + 1, e_minus_one[u]);
        r_times_k[v] += r_times_k[u] + h_times_k(e_minus_one[u]);
    }
    dp[v] = h_times_k(e_minus_one[v] + 1) + r_times_k[v];
}

void solve() {
    int n;
    cin >> n >> k;
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        cin >> a >> b;
        --a;
        --b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    dfs(0, -1);
    Chain ans;
    for (auto u : g[0]) {
        ans += dp[u];
    }
    mint x = ans.Mint();
    x *= mint::Inverse(k.Mint());
    cout << x << endl;
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
