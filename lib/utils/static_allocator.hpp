#ifndef CODE_FOR_REUSE_UTILS_STATIC_ALLOCATOR_HPP_
#define CODE_FOR_REUSE_UTILS_STATIC_ALLOCATOR_HPP_

#include <utility>

template <typename T, int N>
struct StaticAllocator {
    template <typename... Args>
    static T* Allocate(Args&&... args) {
        buf_[ptr_] = T(std::forward<Args>(args)...);
        return buf_ + ptr_++;
    }

    static void Reset() {
        ptr_ = 0;
    }

    static inline T buf_[N];
    static inline int ptr_ = 0;
};

#endif  // CODE_FOR_REUSE_UTILS_STATIC_ALLOCATOR_HPP_