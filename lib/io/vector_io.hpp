#ifndef CODE_FOR_REUSE_IO_VECTOR_IO_HPP_
#define CODE_FOR_REUSE_IO_VECTOR_IO_HPP_

#include <iostream>
#include <vector>

template <class T>
std::istream& operator>>(std::istream& is, std::vector<T>& a) {
    for (auto& x : a) {
        is >> x;
    }
    return is;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& a) {
    bool is_first = true;
    for (const auto& x : a) {
        if (is_first) {
            is_first = false;
        } else {
            os << ' ';
        }
        os << x;
    }
    return os << std::endl;
}

#endif  // CODE_FOR_REUSE_IO_VECTOR_IO_HPP_