#pragma once

#include <bits/stdc++.h>

using namespace std;

vector<int> suffix_array(const auto& s, bool sort_suffixes=true) {
    int n = s.size() + sort_suffixes;
    auto [mn, mx] = minmax_element(s.begin(), s.begin() + n);
    vector<int> c(n), cur(n), sa(n), cnt(max<int>(*mx - *mn + 1, n) + 1);
    iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; ++i) {
        c[i] = s[i] - *mn + 1;
    }
    ranges::sort(sa, [&](int i, int j) { return c[i] < c[j]; });
    for (int k = 1; k < n; k <<= 1) {
        ranges::fill(cnt, 0);
        for (int x : c) cnt[x]++;
        for (int i = 1; i < cnt.size(); ++i) cnt[i] += cnt[i - 1];
        for (int i : sa) {
            i -= k;
            if (i < 0) i += n;
            int c2 = c[i] - 1;
            cur[cnt[c2]++] = i;
        }
        swap(cur, sa);
        int x = -1, y = -1, p = 0;
        for (int i : sa) {
            int nxt = i + k;
            if (nxt >= n) nxt -= n;
            if (c[i] != x || c[nxt] != y) {
                x = c[i], y = c[nxt], p++;
            }
            cur[i] = p;
        }
        swap(cur, c);
    }
    return sa;
}

inline vector<int> find_lcp(const string& s, const vector<int>& sa) {
    int n = sa.size();
    vector<int> pos(n), lcp(n);
    for (int i = 0; i < n; ++i) pos[sa[i]] = i;
    int l = 0;
    for (int i = 0; i < n; ++i) {
        if (pos[i] == n - 1) {
            l = 0;
        } else {
            while (s[(i + l) % n] == s[(sa[pos[i] + 1] + l) % n]) ++l;
            lcp[pos[i]] = l;
            l = max(0, l - 1);
        }
    }
    return lcp;
}
