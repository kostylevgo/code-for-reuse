#ifndef CODE_FOR_REUSE_IO_FILE_HPP_
#define CODE_FOR_REUSE_IO_FILE_HPP_

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>

inline namespace FileIO {

inline void setIn(const std::string& s) {
    freopen(s.c_str(), "r", stdin);
}

inline void setOut(const std::string& s) {
    freopen(s.c_str(), "w", stdout);
}

inline void setIO(const std::string& s = "") {
#ifndef BLAZINGIO
    std::cin.tie(nullptr)->sync_with_stdio(false);  // unsync C / C++ I/O streams
    std::cout << std::setprecision(12) << std::fixed;
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

#endif  // CODE_FOR_REUSE_IO_FILE_HPP_