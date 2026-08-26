#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_BOTTOM_UP_SEGMENT_TREE_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_BOTTOM_UP_SEGMENT_TREE_HPP_

#include <bit>
#include <type_traits>
#include <vector>

#include "no_push.hpp"
#include "segment.hpp"

template <typename T>
struct BottomUpSegmentTree {
    explicit BottomUpSegmentTree(int n) : n_(n), h_(std::bit_width<size_t>(n) - 1), s_(2 * n_) {
    }

    BottomUpSegmentTree(int n, auto&& initializer) : BottomUpSegmentTree(n) {
        for (int i = 0; i < n; ++i) {
            s_[i + n] = T{initializer(i)};
        }
        for (int i = n - 1; i > 0; --i) {
            s_[i] = s_[2 * i] + s_[2 * i + 1];
        }
    }

    T Get(int l, int r) {
        PushAll(l - 1);
        PushAll(r);
        int actual_le = l;
        int actual_ri = r;
        int len = 1;
        T left{};
        T right{};
        for (l += n_, r += n_; l < r; l >>= 1, r >>= 1, len <<= 1) {
            if (l & 1) {
                Segment s{actual_le, actual_le + len};
                Push(l, s);
                left = left + s_[l];
                ++l;
                actual_le += len;
            }
            if (r & 1) {
                --r;
                Segment s{actual_ri - len, actual_ri};
                Push(r, s);
                right = s_[r] + right;
                actual_ri -= len;
            }
        }
        return left + right;
    }

    void Push(int l, int r, const auto& push) {
        int l0 = l;
        int r0 = r;

        PushAll(l - 1);
        PushAll(r);
        int actual_le = l;
        int actual_ri = r;
        int len = 1;
        for (l += n_, r += n_; l < r; l >>= 1, r >>= 1, len <<= 1) {
            if (l & 1) {
                Segment s{actual_le, actual_le + len};
                push.Push(s_[l], s);
                Push(l, s);
                ++l;
                actual_le += len;
            }
            if (r & 1) {
                --r;
                Segment s{actual_ri - len, actual_ri};
                push.Push(s_[r], s);
                Push(r, s);
                actual_ri -= len;
            }
        }
        Pull(l0 - 1);
        Pull(r0);
    }

  private:
    void Push(int i, Segment<int> s) {
        if (i < n_) {
            s_[i].Push(s_[2 * i], s);
            s_[i].Push(s_[2 * i + 1], s);
        }
        s_[i].Push(s);
    }

    void PushAll(int i) {
        if (i < 0 || i >= n_) {
            return;
        }
        int i0 = i;
        i += n_;
        for (int j = h_ - 1; j >= 0; --j) {
            int l = i0 >> j << j;
            Segment s{l, l + (1 << j)};
            Push(i >> j, s);
        }
    }

    void Pull(int i) {
        if (i < 0 || i >= n_) {
            return;
        }
        int i0 = i;
        i += n_;
        int len = 1;
        while (i > 1) {
            i >>= 1;
            len <<= 1;
            int l = i0 / len * len;
            Push(2 * i, Segment{l, l + len / 2});
            Push(2 * i + 1, Segment{l + len / 2, l + len});
            s_[i] = s_[2 * i] + s_[2 * i + 1];
        }
    }

  private:
    int n_;
    int h_;
    std::vector<T> s_;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_BOTTOM_UP_SEGMENT_TREE_HPP_