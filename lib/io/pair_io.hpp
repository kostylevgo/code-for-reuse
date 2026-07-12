#ifndef CODE_FOR_REUSE_IO_PAIR_IO_HPP_
#define CODE_FOR_REUSE_IO_PAIR_IO_HPP_

#include <iostream>
#include <utility>

template <class T1, class T2>
std::istream& operator>>(std::istream& is, std::pair<T1, T2>& a) {
    return is >> a.first >> a.second;
}

template <class T1, class T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& a) {
    return os << '(' << a.first << ',' << ' ' << a.second << ')';
}

#endif  // CODE_FOR_REUSE_IO_PAIR_IO_HPP_