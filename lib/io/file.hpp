#pragma once

inline namespace FileIO {

inline void setIn(const string& s) { freopen(s.c_str(), "r", stdin); }

inline void setOut(const string& s) { freopen(s.c_str(), "w", stdout); }

inline void setIO(const string& s = "") {
#ifndef BLAZINGIO
    cin.tie(0)->sync_with_stdio(0);  // unsync C / C++ I/O streams
    cout << setprecision(12) << fixed;
    // cin.exceptions(cin.failbit);
    // throws exception when do smth illegal
    // ex. try to read letter into int
#endif
    if (!s.empty()) setIn(s + ".in"), setOut(s + ".out");  // for old USACO
#ifdef LOCAL
    else setIn("../in.txt"), setOut("../out.txt");
#endif
}

}  // namespace FileIO
