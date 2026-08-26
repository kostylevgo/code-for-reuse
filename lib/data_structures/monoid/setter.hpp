#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_SETTER_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_SETTER_HPP_

template <typename T>
struct Setter {
    void Push(auto& y, auto) const {
        y = x;
    }

    T x;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_SETTER_HPP_