// https://codeforces.com/problemset/problem/319/C

#ifndef LOCAL
#pragma GCC optimize("O3")
#endif
#include <bits/stdc++.h>
#define endl '\n'
using namespace std;
using ll = int64_t;

const ll INF18 = 1'000'000'000'000'000'018;

#include "data_structures/cht.hpp"

// void solve() {
//     int n;
//     cin >> n;
//     vector<int> a(n);
//     for (auto& x : a) cin >> x;
//     vector<int> b(n);
//     for (auto& x : b) cin >> x;
//     vector<__int128> dp(n);
//     dp[0] = 0;
//     MinCHT cht;
//     cht.Add({b[0], dp[0]});
//     for (int i = 1; i < n; ++i) {
//         dp[i] = cht(a[i]);
//         cht.Add({b[i], dp[i]});
//     }
//     cout << (int64_t)dp.back() << endl;
// }

using Line = LinearFunction<__int128>;

void solve() {
    vector<pair<int, MinCHT<__int128>>> x;
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto& x : a) cin >> x;
    vector<int> b(n);
    for (auto& x : b) cin >> x;
    vector<ll> dp(n);
    dp[0] = 0;
    auto add = [&](Line l) {
        MinCHT<__int128> v;
        v.Add(l);
        x.push_back({1, v});
        while (x.size() >= 2 && x.back().first == x[x.size() - 2].first) {
            auto v0 = x.back();
            x.pop_back();
            auto v1 = x.back();
            x.pop_back();
            x.emplace_back();
            x.back().first = v0.first + 1;
            MergeCHT(v0.second, v1.second, x.back().second);
        }
    };
    add({b[0], dp[0]});
    auto get = [&](ll p) {
        ll ans = INF18;
        for (auto& v : x) {
            ans = min(ans, (ll)v.second(p));
        }
        return ans;
    };
    for (int i = 1; i < n; ++i) {
        dp[i] = get(a[i]);
        add({b[i], dp[i]});
    }
    cout << dp.back() << endl;
}

int main() {
#ifdef LOCAL
    freopen("../in.txt", "r", stdin);
    freopen("../out.txt", "w", stdout);
    auto start = clock();
#else
    cin.tie(nullptr)->sync_with_stdio(false);
#endif
    int tt = 1;
    // cin >> tt;
    while (tt--) solve();
#ifdef LOCAL
    cerr << fixed << setprecision(3) << "TIME: " << 1e3 * (clock() - start) / CLOCKS_PER_SEC << " ms" << endl;
#endif
    return 0;
}
