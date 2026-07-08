#ifndef PAIR_IO_HPP_
#define PAIR_IO_HPP_

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

#endif
