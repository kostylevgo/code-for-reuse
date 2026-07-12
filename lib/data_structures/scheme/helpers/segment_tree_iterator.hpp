#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_SCHEME_HELPERS_SEGMENT_TREE_ITERATOR_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_SCHEME_HELPERS_SEGMENT_TREE_ITERATOR_HPP_

#include <array>
#include <concepts>

struct Include {
    explicit Include(int x) : x(x) {
    }

    [[nodiscard]] int ConvertLeft() const {
        return x;
    }

    [[nodiscard]] int ConvertRight() const {
        return x + 1;
    }

    int x;
};

struct Exclude {
    explicit Exclude(int x) : x(x) {
    }

    [[nodiscard]] int ConvertLeft() const {
        return x + 1;
    }

    [[nodiscard]] int ConvertRight() const {
        return x;
    }

    int x;
};

template <typename T>
concept SegmentBound = requires(T t) {
    { t.ConvertLeft() } -> std::same_as<int>;
    { t.ConvertRight() } -> std::same_as<int>;
};

struct Segment {
    template <SegmentBound Left, SegmentBound Right>
    explicit Segment(Left left, Right right) : left_(left.ConvertLeft()), right_(right.ConvertRight()) {
    }

    explicit Segment(int left, int right) : left_(left), right_(right) {
    }

    explicit Segment(int x) : left_(x), right_(x + 1) {
    }

    [[nodiscard]] Segment CutLeft(int x) const {
        return Segment(x, right_);
    }

    [[nodiscard]] Segment CutRight(int x) const {
        return Segment(left_, x);
    }

    [[nodiscard]] int LeftBound() const {
        return left_;
    }

    [[nodiscard]] int RightBound() const {
        return right_;
    }

    [[nodiscard]] bool Contains(Segment other) const {
        return LeftBound() <= other.LeftBound() && other.RightBound() <= RightBound();
    }

    [[nodiscard]] bool ToTheLeftFrom(int x) const {
        return RightBound() <= x;
    }

    [[nodiscard]] bool ToTheLeftFrom(Segment other) const {
        return ToTheLeftFrom(other.LeftBound());
    }

    [[nodiscard]] bool ToTheRightFrom(int x) const {
        return LeftBound() >= x;
    }

    [[nodiscard]] bool ToTheRightFrom(Segment other) const {
        return ToTheRightFrom(other.RightBound());
    }

    [[nodiscard]] bool Outside(Segment other) const {
        return ToTheLeftFrom(other) || ToTheRightFrom(other);
    }

    [[nodiscard]] int Middle() const {
        return (LeftBound() + RightBound()) / 2;
    }

    [[nodiscard]] int Length() const {
        return RightBound() - LeftBound();
    }

    [[nodiscard]] bool IsSingular() const {
        return Length() == 1;
    }

    [[nodiscard]] int Position() const {
        return LeftBound();
    }

    bool operator==(const Segment&) const = default;

  private:
    int left_;
    int right_;
};

struct SegmentTreeIterator : Segment {
    static SegmentTreeIterator Root(int n) {
        return {Segment(0, n), 0};
    }

    [[nodiscard]] SegmentTreeIterator Left() const {
        return {CutRight(Middle()), 2 * index_ + 1};
    }

    [[nodiscard]] SegmentTreeIterator Right() const {
        return {CutLeft(Middle()), 2 * index_ + 2};
    }

    [[nodiscard]] auto Children() const {
        return std::array{Left(), Right()};
    }

    operator int() const {
        return index_;
    }

  private:
    SegmentTreeIterator(Segment s, int i) : Segment(s), index_(i) {
    }

  private:
    int index_;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_SCHEME_HELPERS_SEGMENT_TREE_ITERATOR_HPP_