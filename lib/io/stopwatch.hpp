#pragma once

#include "debug.hpp"

#include <bits/stdc++.h>

using namespace std;

class Clock {
public:
    // https://stackoverflow.com/questions/47980498/accurate-c-c-clock-on-a-multi-core-processor-with-auto-overclock?noredirect=1&lq=1
    double time_elapsed() {
        return chrono::duration<double>(chrono::high_resolution_clock::now() -
                                        beg_time)
            .count();
    }

private:
    const chrono::time_point<chrono::high_resolution_clock> beg_time = chrono::high_resolution_clock::now();
};

struct PrintCallback {
    template <auto& is>
    class FlagGuard {
    public:
        FlagGuard(): flags(is.flags()), precision(is.precision()) {}

        ~FlagGuard() {
            is.flags(flags);
            is.precision(precision);
        }

    private:
        decltype(is.flags()) flags;
        decltype(is.precision()) precision;
    };

    void operator()(double t) const {
        FlagGuard<cerr> guard;
        cerr << setprecision(3) << fixed;
        Writer<cerr, false, true>{}.print_with_sep(" ", "TIME:", t, "sec");
    }
};

template <class Cb = PrintCallback>
class Stopwatch : public Clock {
public:
    ~Stopwatch() {
        Cb()(time_elapsed());
    }
};
