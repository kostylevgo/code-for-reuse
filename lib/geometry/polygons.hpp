#ifndef CODE_FOR_REUSE_GEOMETRY_POLYGONS_HPP_
#define CODE_FOR_REUSE_GEOMETRY_POLYGONS_HPP_

#include <utility>
#include <vector>

#include "pair_operators.hpp"

template <class T>
T TwiceSquare(const std::vector<std::pair<T, T>>& b) {
    T square = 0;
    for (int i = 0; i < b.size(); ++i) {
        int nxt = i + 1;
        if (nxt == b.size()) nxt = 0;
        square += b[i] % b[nxt];
    }
    return abs(square);
}

#endif  // CODE_FOR_REUSE_GEOMETRY_POLYGONS_HPP_