#pragma once

#include "pair_operators.hpp"

#include <bits/stdc++.h>

using ld = long double;

struct Line {
    pair<ld, ld> p;
    pair<ld, ld> shift;
};

inline bool is_in_right_half(pair<ld, ld> a) {
    return a.first > 0 || (a.first == 0 && a.second > 0);
}

inline pair<ld, ld> intersection(Line a, Line b) {
    ld coeff = (a.p - b.p) % a.shift / (b.shift % a.shift);
    return b.p + b.shift * coeff;
}

inline void hull_reverse(vector<Line>& lower) {
    reverse(lower.begin(), lower.end());
    for (auto& line : lower) {
        line.p.first = -line.p.first;
        line.shift.second = -line.shift.second;
    }
}

inline vector<Line> half_plane_intersection(const vector<Line>& lines) {
    const ld EPS = 1e-8;
    const ld INF = 1e18;

    vector<Line> upper, lower;
    vector<Line> for_upper, for_lower;
    for (auto line : lines) {
        if (is_in_right_half(line.shift)) {
            for_upper.push_back(line);
        } else {
            for_lower.push_back(line);
        }
    }
    auto process_hull = [EPS](vector<Line>& hull, vector<Line>& for_hull) {
        sort(for_hull.begin(), for_hull.end(), [](Line a, Line b) {return HalfPlanePolarComparator<ld>()(a.shift, b.shift);});
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
                pair<ld, ld> now_inter = intersection(base, now);
                pair<ld, ld> nxt_inter = intersection(base, nxt);
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
            ld upper_inter = (upper.size() == 1 ? -INF : intersection(upper.back(), upper[upper.size() - 2]).first);
            ld lower_inter = (lower.size() == 1 ? -INF : intersection(lower.back(), lower[lower.size() - 2]).first);
            ld max_inter = max(upper_inter, lower_inter);
            vector<Line>& chosen_hull = upper_inter == max_inter ? upper : lower;
            if (max_inter == -INF) {
                break;
            }
            pair<ld, ld> chosen_inter = intersection(chosen_hull.back(), chosen_hull[chosen_hull.size() - 2]);
            if (upper.back().shift % lower.back().shift < EPS) {
                break;
            }
            pair<ld, ld> right_inter = intersection(upper.back(), lower.back());
            ld coord_chosen = (chosen_inter - chosen_hull.back().p) * chosen_hull.back().shift;
            ld coord_right = (right_inter - chosen_hull.back().p) * chosen_hull.back().shift;
            if (coord_chosen < coord_right) {
                break;
            }
            chosen_hull.pop_back();
        }
        hull_reverse(upper);
        hull_reverse(lower);
    }
    for (int ind = lower.size(); ind--;) {
        upper.push_back(lower[ind]);
    }
    return upper;
}
