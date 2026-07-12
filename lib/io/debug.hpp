#ifndef CODE_FOR_REUSE_IO_DEBUG_HPP_
#define CODE_FOR_REUSE_IO_DEBUG_HPP_

#include "io.hpp"

inline namespace Debug {
template <typename... Args>
void err(Args... args) {
    Writer<std::cerr, true, false>{}.print_with_sep(" | ", args...);
}

template <typename... Args>
void errn(Args... args) {
    Writer<std::cerr, true, true>{}.print_with_sep(" | ", args...);
}

void err_prefix(const std::string& func, int line, const std::string& args) {
    std::cerr << "\033[0;31m\u001b[1mDEBUG\033[0m"
              << " | "
              << "\u001b[34m" << func << "\033[0m"
              << ":"
              << "\u001b[34m" << line << "\033[0m"
              << " - "
              << "[" << args << "] = ";
}

#ifdef LOCAL
#define dbg(args...) err_prefix(__FUNCTION__, __LINE__, #args), err(args)
#define dbgn(args...) err_prefix(__FUNCTION__, __LINE__, #args), errn(args)
#else
#define dbg(...)
#define dbgn(args...)
#endif

}  // namespace Debug

#endif  // CODE_FOR_REUSE_IO_DEBUG_HPP_