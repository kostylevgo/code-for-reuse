#include <bits/stdc++.h>
#ifndef LOCAL
#define endl '\n'
#endif
// #define int int64_t

using namespace std;

#include "graph/flow/flow.hpp"
#include "io/vector_io.hpp"

void solve() {
    int n, m;
    cin >> n >> m;
    Flow f;
    vector<Vertex> in(n), out(n);
    in[0] = f.AddVertex();
    out[0] = f.S;
    in[n - 1] = f.T;
    out[n - 1] = f.AddVertex();
    for (int i = 1; i < n - 1; ++i) {
        in[i] = f.AddVertex();
        out[i] = f.AddVertex();
    }
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        --a; --b;
        f.AddEdge(out[a], in[b], kMax);
        f.AddEdge(out[b], in[a], kMax);
    }
    vector<Edge> edges(n);
    for (int i = 0; i < n; ++i) {
        Cap c;
        cin >> c;
        edges[i] = f.AddEdge(in[i], out[i], c);
    }
    Cap cost = f.PushRelabel();
    auto cut = f.Cut();
    ranges::sort(cut);
    vector<int> ans;
    for (int i = 1; i < n - 1; ++i) {
        auto it = ranges::lower_bound(cut, edges[i]);
        if (it != cut.end() && *it == edges[i]) {
            ans.push_back(i + 1);
        }
    }
    cout << cost << endl << ans.size() << endl << ans;
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
