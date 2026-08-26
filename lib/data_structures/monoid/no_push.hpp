#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_NO_PUSH_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_NO_PUSH_HPP_

#include <utility>

struct NoPushBase {
    template <typename... Args>
    void Push(Args&&...) const {
    }
};

template <typename T>
struct NoPush : NoPushBase {
    template <typename... Args>
    explicit NoPush(Args&&... args) : x(std::forward<Args>(args)...) {
    }

    NoPush& operator=(const T& y) {
        x = y;
        return *this;
    }

    NoPush& operator=(T&& y) {
        x = std::move(y);
        return *this;
    }

    friend NoPush operator+(NoPush a, NoPush b) {
        return NoPush(a.x + b.x);
    }

    operator T() const {
        return x;
    }

    T x;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_NO_PUSH_HPP_