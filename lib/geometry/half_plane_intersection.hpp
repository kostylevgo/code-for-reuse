#ifndef CODE_FOR_REUSE_GEOMETRY_HALF_PLANE_INTERSECTION_HPP_
#define CODE_FOR_REUSE_GEOMETRY_HALF_PLANE_INTERSECTION_HPP_

#include <algorithm>
#include <vector>

#include "pair_operators.hpp"

using ld = long double;

struct Line {
    std::pair<ld, ld> p;
    std::pair<ld, ld> shift;
};

inline bool IsInRightHalf(std::pair<ld, ld> a) {
    return a.first > 0 || (a.first == 0 && a.second > 0);
}

inline std::pair<ld, ld> Intersection(Line a, Line b) {
    ld coeff = (a.p - b.p) % a.shift / (b.shift % a.shift);
    return b.p + b.shift * coeff;
}

inline void ReverseHull(std::vector<Line>& lower) {
    std::ranges::reverse(lower);
    for (auto& line : lower) {
        line.p.first = -line.p.first;
        line.shift.second = -line.shift.second;
    }
}

inline std::vector<Line> HalfPlaneIntersection(const std::vector<Line>& lines) {
    const ld EPS = 1e-8;
    const ld INF = 1e18;

    std::vector<Line> upper, lower;
    std::vector<Line> for_upper, for_lower;
    for (auto line : lines) {
        if (IsInRightHalf(line.shift)) {
            for_upper.push_back(line);
        } else {
            for_lower.push_back(line);
        }
    }
    auto process_hull = [EPS](std::vector<Line>& hull, std::vector<Line>& for_hull) {
        sort(for_hull.begin(), for_hull.end(),
             [](Line a, Line b) { return HalfPlanePolarComparator<ld>()(a.shift, b.shift); });
        for (const auto line : for_hull) {
            if (!hull.empty() && abs(hull.back().shift % line.shift) < EPS) {
                if ((line.p - hull.back().p) % line.shift >= -EPS) {
                    hull.pop_back();
                } else {
                    continue;
                }
            }
            while (hull.size() >= 2) {
                Line base = hull[hull.size() - 2];
                Line now = hull.back();
                Line nxt = line;
                std::pair<ld, ld> now_inter = Intersection(base, now);
                std::pair<ld, ld> nxt_inter = Intersection(base, nxt);
                if ((now_inter - base.p) * base.shift > (nxt_inter - base.p) * base.shift) {
                    break;
                }
                hull.pop_back();
            }
            hull.push_back(line);
        }
    };
    process_hull(lower, for_lower);
    process_hull(upper, for_upper);
    reverse(upper.begin(), upper.end());
    for (auto& line : lower) {
        line.shift = {-line.shift.first, -line.shift.second};
    }
    for (int it = 0; it < 2; it++) {
        while (true) {
            ld upper_inter = (upper.size() == 1 ? -INF : Intersection(upper.back(), upper[upper.size() - 2]).first);
            ld lower_inter = (lower.size() == 1 ? -INF : Intersection(lower.back(), lower[lower.size() - 2]).first);
            ld max_inter = std::max(upper_inter, lower_inter);
            std::vector<Line>& chosen_hull = upper_inter == max_inter ? upper : lower;
            if (max_inter == -INF) {
                break;
            }
            std::pair<ld, ld> chosen_inter = Intersection(chosen_hull.back(), chosen_hull[chosen_hull.size() - 2]);
            if (upper.back().shift % lower.back().shift < EPS) {
                break;
            }
            std::pair<ld, ld> right_inter = Intersection(upper.back(), lower.back());
            ld coord_chosen = (chosen_inter - chosen_hull.back().p) * chosen_hull.back().shift;
            ld coord_right = (right_inter - chosen_hull.back().p) * chosen_hull.back().shift;
            if (coord_chosen < coord_right) {
                break;
            }
            chosen_hull.pop_back();
        }
        ReverseHull(upper);
        ReverseHull(lower);
    }
    for (int ind = lower.size(); ind--;) {
        upper.push_back(lower[ind]);
    }
    return upper;
}

#endif  // CODE_FOR_REUSE_GEOMETRY_HALF_PLANE_INTERSECTION_HPP_