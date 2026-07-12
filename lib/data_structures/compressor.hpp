#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_COMPRESSOR_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_COMPRESSOR_HPP_

#include <algorithm>
#include <vector>

template <typename T>
struct Compressor : private std::vector<T> {
    using Base = std::vector<T>;

  public:
    explicit Compressor(std::vector<T> v) : Base(v) {
        std::ranges::sort(begin(), end());
        auto [b, e] = std::ranges::unique(begin(), end());
        Base::erase(b, e);
    }

    using Base::size;
    using Base::operator[];

    using Base::begin;
    using Base::end;

    using Base::cbegin;
    using Base::cend;

    int Compress(const T& item) const {
        return static_cast<int>(std::ranges::lower_bound(*this, item) - begin());
    }

    int operator()(const T& item) const {
        return Compress(item);
    }

    const T& Decompress(int pos) const {
        return Base::operator[](pos);
    }

  private:
    auto begin() {
        return Base::begin();
    }

    auto end() {
        return Base::end();
    }
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_COMPRESSOR_HPP_