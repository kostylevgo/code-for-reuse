#ifndef SCHEME_HPP_
#define SCHEME_HPP_

#include "action.hpp"

template <typename S>
struct SchemeTraits {
private:
    static constexpr auto HasGet(auto& s) {
        if constexpr (requires {s.Get();}) {
            if constexpr (std::is_lvalue_reference_v<decltype(s.Get())>) {
                return std::true_type{};
            } else {
                return std::false_type{};
            }
        } else {
            return std::false_type{};
        }
    }

    static constexpr auto HasNoPush() {
        if constexpr (requires { {S::kNoPush} -> std::convertible_to<bool>; }) {
            return S::kNoPush;
        }
        return false_type{};
    }

    static_assert(std::is_same_v<decltype(HasGet(std::declval<S&>())),
                                 decltype(HasGet(std::declval<const S&>()))
                                 >, "Different Get() check results for S& and const S&");

public:
    static auto& ToMonoid(const S& s) {
        if constexpr (HasGet(s)) {
            return s.Get();
        } else {
            return s;
        }
    }

    static auto& ToMonoid(S& s) {
        if constexpr (HasGet(s)) {
            return s.Get();
        } else {
            return s;
        }
    }

    static constexpr bool kNoPush = HasNoPush();
};

template <typename S>
concept Scheme = Respects<S, S>
&& requires(S& s) { s.Reset(); }
&& requires(S& s, SegmentTreeIterator i) { PushTraits::SelfPush(s, i); }
&& requires(const S s) { SchemeTraits<S>::ToMonoid(s); }
&& Monoid<std::decay_t<decltype(SchemeTraits<S>::ToMonoid(std::declval<S&>()))>>
;

template <typename T>
struct NoPush {
    static constexpr bool kNoPush = true;

    void Reset() {
        // pass
    }

    void Push(T&) const {
        // pass
    }

    void SelfPush() {
        // pass
    }
};

template <typename T>
struct Setter {
    template <typename U>
    void Push(U& other) const {
        other = U{t};
    }

    T& t;
};

#endif
