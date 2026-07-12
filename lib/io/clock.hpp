#ifndef CODE_FOR_REUSE_IO_CLOCK_HPP_
#define CODE_FOR_REUSE_IO_CLOCK_HPP_

#include <chrono>
#include <iomanip>
#include <iostream>

#include "debug.hpp"

class Clock {
  public:
    // https://stackoverflow.com/questions/47980498/accurate-c-c-clock-on-a-multi-core-processor-with-auto-overclock?noredirect=1&lq=1
    double TimeElapsed() {
        return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - begin_time).count();
    }

  private:
    const std::chrono::time_point<std::chrono::high_resolution_clock> begin_time =
        std::chrono::high_resolution_clock::now();
};

template <class Cb>
class Stopwatch : public Clock {
  public:
    ~Stopwatch() {
        Cb()(TimeElapsed());
    }
};

struct PrintCallback {
    template <auto& is>
    class FlagGuard {
      public:
        FlagGuard() : flags(is.flags()), precision(is.precision()) {
        }

        ~FlagGuard() {
            is.flags(flags);
            is.precision(precision);
        }

      private:
        decltype(is.flags()) flags;
        decltype(is.precision()) precision;
    };

    void operator()(double t) const {
        FlagGuard<std::cerr> guard;
        std::cerr << std::setprecision(3) << std::fixed;
        Writer<std::cerr, false, true>{}.print_with_sep(" ", "TIME:", t, "sec");
    }
};

using ChimingClock = Stopwatch<PrintCallback>;

#endif  // CODE_FOR_REUSE_IO_CLOCK_HPP_