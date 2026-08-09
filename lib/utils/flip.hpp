#ifndef CODE_FOR_REUSE_UTILS_FLIP_HPP_
#define CODE_FOR_REUSE_UTILS_FLIP_HPP_

auto Flip(auto x) {
    return -x - 1;
}

bool IsFlipped(auto x) {
    return x < 0;
}

auto NoFlips(auto x) {
    return IsFlipped(x) ? Flip(x) : x;
}

#endif  // CODE_FOR_REUSE_UTILS_FLIP_HPP_