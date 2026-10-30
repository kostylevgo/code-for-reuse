//https://codeforces.com/gym/105446/problem/C

#ifndef LOCAL
#pragma GCC optimize("O3")
#endif
#include <bits/stdc++.h>
#define endl '\n'
//#define int int64_t

#include "optim/adam.hpp"

using namespace std;

typedef int64_t ll;
typedef long double ld;

constexpr int INF = 1'000'000'009;
constexpr ll INF18 = 1'000'000'000'000'000'018;

template <class T1, class T2> istream& operator>>(istream& is, pair<T1, T2>& a) {return is >> a.first >> a.second;}
template <class T1, class T2> ostream& operator<<(ostream& os, const pair<T1, T2>& a) {return os << '(' << a.first << ", " << a.second << ')';}
template <class T> istream& operator>>(istream& is, vector<T>& a) {for (auto &x : a) is >> x; return is;}
template <class T> ostream& operator<<(ostream& os, const vector<T>& a) {for (auto x : a) os << x << ' '; return os << endl;}

void solve() {
    int n;
    cin >> n;
    pair<ld, ld> start, finish;
    vector<pair<pair<ld, ld>, pair<ld, ld>>> a(n);
    cin >> start >> a >> finish;

    const ld EPS = 1e-8;

    auto pt_dist = [](pair<ld, ld> x, pair<ld, ld> y) {
        x.first -= y.first;
        x.second -= y.second;
        return sqrtl(x.first * x.first + x.second * x.second);
    };

    auto dist = [&](const vector<ld>& vars) {
        ld ans = 0;
        pair<ld, ld> last_pt = start;
        for (int i = 0; i < n; ++i) {
            pair<ld, ld> now_pt = {a[i].first.first * vars[i] + a[i].second.first * (1 - vars[i]),
            a[i].first.second * vars[i] + a[i].second.second * (1 - vars[i])};
            ans += pt_dist(last_pt, now_pt);
            last_pt = now_pt;
        }
        ans += pt_dist(last_pt, finish);
        return ans;
    };

    auto grad = [&](vector<ld> vars) {
        vector<ld> ans(n);
        ld now_dist = dist(vars);
        for (int v = 0; v < n; ++v) {
            ld delta = EPS;
            if (vars[v] + delta > 1) {
                delta = -delta;
            }
            vars[v] += delta;
            ld new_dist = dist(vars);
            ans[v] = (new_dist - now_dist) / delta;
            vars[v] -= delta;
        }
        return ans;
    };

    ld ans = INF18;
    mt19937 gen(31);
    auto distr = uniform_real_distribution<ld>(0, 1);

    Adam<> adam(n, 1);
    for (auto& x : adam.variables) x = distr(gen);

    for (int j = 0; adam && j < 1000; ++j) {
        auto now_ans = dist(adam.variables);
        ans = min(ans, now_ans);
        auto g = grad(adam.variables);
        adam.step(now_ans, g);
        for (int k = 0; k < n; ++k) {
            if (adam.variables[k] < 0) adam.variables[k] = 0;
            if (adam.variables[k] > 1) adam.variables[k] = 1;
        }
    }
    cout << setprecision(12) << fixed << ans << endl;
}

int32_t main() {
#ifdef LOCAL
    freopen("../in.txt", "r", stdin);
    freopen("../out.txt", "w", stdout);
    const auto start = clock();
#endif
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    int ttt = 1;
    //cin >> ttt;
    while (ttt --> 0) solve();
#ifdef LOCAL
    cerr << setprecision(3) << fixed << endl << "TIME: " << static_cast<double>(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
#endif
}