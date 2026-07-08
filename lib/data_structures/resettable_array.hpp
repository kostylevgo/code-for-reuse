#pragma once

#include <array>
#include <vector>

template <class T, size_t N>
class ResettableArray : public array<T, N> {
public:
    ResettableArray(): array<T, N>() {}

    T& operator[](size_t i) {
        if (timestamps_[i] < t_) {
            timestamps_[i] = t_;
            (*this)[i] = T{};
        }
        return (*this)[i];
    }

    void Reset() {
        ++t_;
    }

private:
    int t_ = 1;
    array<int, N> timestamps_{};
};