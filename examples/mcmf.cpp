#include "graph/flow/mcmf.hpp"

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

template <class T1, class T2>
istream& operator>>(istream& is, pair<T1, T2>& a) {
    return is >> a.first >> a.second;
}

template <class T1, class T2>
ostream& operator<<(ostream& os, const pair<T1, T2>& a) {
    return os << '(' << a.first << ", " << a.second << ')';
}

template <class T>
istream& operator>>(istream& is, vector<T>& a) {
    for (auto& x : a) is >> x;
    return is;
}

template <class T>
ostream& operator<<(ostream& os, const vector<T>& a) {
    for (auto x : a) os << x << ' ';
    return os << endl;
}

void solve() {
    MCMF x;
    x.Run();
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
