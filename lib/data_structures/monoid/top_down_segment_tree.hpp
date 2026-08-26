#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_TOP_DOWN_SEGMENT_TREE_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_TOP_DOWN_SEGMENT_TREE_HPP_

#include <bit>
#include <vector>

#include "segment.hpp"

template <typename T>
struct TopDownSegmentTree {
  private:
    struct TopDownSegmentTreeIterator : Segment<int> {
        TopDownSegmentTreeIterator(int i, int l, int r) : Segment(l, r), i(i) {
        }

        TopDownSegmentTreeIterator Left() const {
            return {2 * i + 1, left, (right + left) / 2};
        }

        TopDownSegmentTreeIterator Right() const {
            return {2 * i + 2, (right + left) / 2, right};
        }

        operator int() const {
            return i;
        }

        bool IsLeaf() const {
            return Length() <= 1;
        }

        int i;
    };

  public:
    explicit TopDownSegmentTree(int n) : n_(n), s_(std::bit_ceil<size_t>(n) * 2) {
    }

    TopDownSegmentTree(int n, auto&& initializer) : TopDownSegmentTree(n) {
        Build(Root(), initializer);
    }

    void Push(int L, int R, const auto& push) {
        PushSegment({L, R}, push, Root());
    }

    T Get(int L, int R) {
        return GetSegment({L, R}, Root());
    }

  private:
    void Push(const TopDownSegmentTreeIterator i) {
        if (!i.IsLeaf()) {
            s_[i].Push(s_[i.Left()], i);
            s_[i].Push(s_[i.Right()], i);
        }
        s_[i].Push(i);
    }

    void Build(const TopDownSegmentTreeIterator i, auto&& initializer) {
        if (i.IsLeaf()) {
            s_[i] = T{initializer(i.left)};
        } else {
            Build(i.Left(), initializer);
            Build(i.Right(), initializer);
            s_[i] = s_[i.Left()] + s_[i.Right()];
        }
    }

    T GetSegment(Segment<int> s, const TopDownSegmentTreeIterator i) {
        Push(i);
        if (s.Contains(i)) {
            return s_[i];
        }
        if (s.Outside(i)) {
            return T{};
        }
        return GetSegment(s, i.Left()) + GetSegment(s, i.Right());
    }

    void PushSegment(Segment<int> s, const auto& push, const TopDownSegmentTreeIterator i) {
        if (s.Contains(i)) {
            push.Push(s_[i], i);
            Push(i);
            return;
        }
        Push(i);
        if (s.Outside(i)) {
            return;
        }
        PushSegment(s, push, i.Left());
        PushSegment(s, push, i.Right());
        s_[i] = s_[i.Left()] + s_[i.Right()];
    }

    TopDownSegmentTreeIterator Root() const {
        return {0, 0, n_};
    }

  private:
    int n_;
    std::vector<T> s_;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_TOP_DOWN_SEGMENT_TREE_HPP_