#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_RESETTABLE_ARRAY_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_RESETTABLE_ARRAY_HPP_

#include "used_counter.hpp"

template <typename T>
class ResettableArray {
  public:
    ResettableArray() = default;

    explicit ResettableArray(int n) : items_(n), used_(n) {
    }

    T& operator[](int i) {
        if (!used_[i]) {
            used_.Use(i);
            items_[i] = T{};
        }
        return items_[i];
    }

    void Reset() {
        used_.Reset();
    }

  private:
    std::vector<T> items_;
    UsedCounter used_;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_RESETTABLE_ARRAY_HPP_