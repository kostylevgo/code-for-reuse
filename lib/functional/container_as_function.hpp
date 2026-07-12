#ifndef CODE_FOR_REUSE_FUNCTIONAL_CONTAINER_AS_FUNCTION_HPP_
#define CODE_FOR_REUSE_FUNCTIONAL_CONTAINER_AS_FUNCTION_HPP_

#include <cstddef>

template <typename T>
class AsFunction {
  public:
    explicit AsFunction(T& container) : container_(container) {
    }

    const auto& operator()(size_t i) const {
        return container_[i];
    }

    auto& operator()(size_t i) {
        return container_[i];
    }

  private:
    T& container_;
};

#endif  // CODE_FOR_REUSE_FUNCTIONAL_CONTAINER_AS_FUNCTION_HPP_