#ifndef TOP_DOWN_SEGMENT_TREE_HPP_
#define TOP_DOWN_SEGMENT_TREE_HPP_

#include "concepts/scheme.hpp"
#include "helpers/segment_tree_iterator.hpp"

#include <vector>

template <Scheme S>
class TopDownSegmentTree : private SchemeTraits<S>, private PushTraits {
    using SchemeTraits<S>::ToMonoid;
    using PushTraits::Push;
    using PushTraits::SelfPush;
    using M = std::decay_t<decltype(ToMonoid(std::declval<S&>()))>;
    using I = SegmentTreeIterator;

public:
    explicit TopDownSegmentTree(size_t n): n_(static_cast<int>(n)), segments_(ArraySize(n)) {}

    TopDownSegmentTree(size_t n, const auto& initializer): TopDownSegmentTree(n) {
        Build(RootIterator(), initializer);
    }

    M At(int i) {
        return Get(Segment(i));
    }

    M Get(auto l, auto r) {
        return Get(Segment(l, r));
    }

    M Get(Segment s) {
        if (RootIterator().Outside(s) || s.Length() <= 0) {
            return M{};
        }
        return GetRecursive(RootIterator(), s);
    }

    template <Respects<S> T>
    void UpdateAt(int i, T&& op) {
        return Update(Segment(i), op);
    }

    template <Respects<S> T>
    void Update(auto l, auto r, T&& op) {
        return Update(Segment(l, r), op);
    }

    template <Respects<S> T>
    void Update(Segment s, T&& op) {
        static_assert(!SchemeTraits<std::decay_t<T>>::kNoPush, "Using an action with no push. Try using Setter instead.");
        if (RootIterator().Outside(s) || s.Length() <= 0) {
            return;
        }
        UpdateRecursive(RootIterator(), s, op);
    }

private:
    [[nodiscard]] I RootIterator() const {
        return I::Root(n_);
    }

    void Push(I i) {
        SelfPush(segments_[i], i);
        if (!i.IsSingular()) {
            for (auto j : i.Children()) {
                Push(segments_[i], segments_[j], i);
            }
        }
        segments_[i].Reset();
    }

    void Build(I i, const auto& initializer) {
        if (i.IsSingular()) {
            ToMonoid(segments_[i]) = M{initializer(i.Position())};
        } else {
            for (auto j : i.Children()) {
                Build(j, initializer);
            }
            ToMonoid(segments_[i]) = M::Combine(ToMonoid(segments_[i.Left()]), ToMonoid(segments_[i.Right()]));
        }
    }

    M GetRecursive(I i, Segment s) {
        while (true) {
            Push(i);
            if (i == s) {
                return ToMonoid(segments_[i]);
            }
            int middle = i.Middle();
            if (s.ToTheLeftFrom(middle)) {
                i = i.Left();
                continue;
            }
            if (s.ToTheRightFrom(middle)) {
                i = i.Right();
                continue;
            }
            return M::Combine(GetRecursive(i.Left(), s.CutRight(middle)), GetRecursive(i.Right(), s.CutLeft(middle)));
        }
    }

    template <Respects<S> T>
    void UpdateRecursive(I i, Segment s, T&& op) {
        if (s.Contains(i)) {
            Push(op, segments_[i], i);
            Push(i);
            return;
        }
        Push(i);
        if (i.Outside(s)) {
            return;
        }
        for (auto j : i.Children()) {
            UpdateRecursive(j, s, op);
        }
        ToMonoid(segments_[i]) = M::Combine(ToMonoid(segments_[i.Left()]), ToMonoid(segments_[i.Right()]));
    }

    static size_t ArraySize(size_t n) {
        return 2 * std::bit_ceil(n) - 1;
    }

private:
    int n_;
    std::vector<S> segments_;
};

#endif
