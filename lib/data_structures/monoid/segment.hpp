#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_SEGMENT_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_SEGMENT_HPP_

template <typename T>
struct Segment {
    bool Outside(const Segment& other) const {
        return left >= other.right || other.left >= right;
    }

    bool Contains(const Segment& other) const {
        return left <= other.left && other.right <= right;
    }

    T Length() const {
        return right - left;
    }

    T left;
    T right;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_MONOID_SEGMENT_HPP_