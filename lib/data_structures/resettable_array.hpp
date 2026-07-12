#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_RESETTABLE_ARRAY_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_RESETTABLE_ARRAY_HPP_

template <typename T, template <typename> typename Container>
class ResettableArray : private Container<T> {
  public:
    ResettableArray() = default;

    explicit ResettableArray(int n) : Container<T>(n), timestamps_(n) {
    }

    T& operator[](int i) {
        if (timestamps_[i] < t_) {
            timestamps_[i] = t_;
            AsContainer()[i] = T{};
        }
        return AsContainer()[i];
    }

    void Reset() {
        ++t_;
    }

  private:
    Container<T>& AsContainer() const {
        return static_cast<Container<T>&>(*this);
    }

  private:
    int t_ = 1;
    Container<int> timestamps_;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_RESETTABLE_ARRAY_HPP_