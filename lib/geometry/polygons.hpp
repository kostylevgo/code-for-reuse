#pragma once

#include "pair_operators.hpp"

template <class T>
T twice_square(const vector<pair<T, T>>& b) {
    T square = 0;
    for (int i = 0; i < b.size(); ++i) {
        int nxt = i + 1;
        if (nxt == b.size()) nxt = 0;
        square += b[i] % b[nxt];
    }
    return abs(square);
}
