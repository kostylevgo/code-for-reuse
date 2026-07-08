// https://codeforces.com/gym/100246, problem B

#include <bits/stdc++.h>
#ifndef LOCAL
#define endl '\n'
#endif
// #define int int64_t

using namespace std;

#include "data_structures/fenwick.hpp"

std::array<std::array<std::array<int, 128>, 128>, 128> stars{};

void solve() {
    int _;
    cin >> _;
    while (true) {
        int t;
        cin >> t;
        if (t == 3) {
            break;
        }
        if (t == 1) {
            int x, y, z, k;
            cin >> x >> y >> z >> k;
            for (auto& vec_x : FenwickAncestors(stars, x)) {
                for (auto& vec_y : FenwickAncestors(vec_x, y)) {
                    for (auto& pos : FenwickAncestors(vec_y, z)) {
                        pos += k;
                    }
                }
            }
        } else {
            array<int, 2> x, y, z;
            cin >> x[0] >> y[0] >> z[0] >> x[1] >> y[1] >> z[1];
            --x[0];
            --y[0];
            --z[0];
            int64_t ans = 0;
            for (int mask = 0; mask < 8; ++mask) {
                int xi = x[mask >> 2 & 1];
                int yi = y[mask >> 1 & 1];
                int zi = z[mask & 1];
                int sign = popcount<uint32_t>(mask) % 2 * 2 - 1;
                int sum = 0;
                for (auto& vec_x : FenwickPrefix(stars, xi)) {
                    for (auto& vec_y : FenwickPrefix(vec_x, yi)) {
                        for (auto& pos : FenwickPrefix(vec_y, zi)) {
                            sum += pos;
                        }
                    }
                }
                ans += sum * sign;
            }
            cout << ans << endl;
        }
    }
}

int32_t main() {
#ifdef LOCAL
    freopen("../in.txt", "r", stdin);
    freopen("../out.txt", "w", stdout);
    const auto start = clock();
#else
    freopen("./stars.in", "r", stdin);
    freopen("./stars.out", "w", stdout);
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
