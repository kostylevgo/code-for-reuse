#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_SCHEME_CONCEPTS_ACTION_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_SCHEME_CONCEPTS_ACTION_HPP_

#include "../helpers/segment_tree_iterator.hpp"
#include "monoid.hpp"

template <typename... Ts>
struct ActionSequence;

struct PushTraits {
    template <typename A, typename G>
    static void Push(const A& action, G& down, SegmentTreeIterator iter)
        requires(requires { action.Push(down, iter); })
    {
        action.Push(down, iter);
    }

    template <typename A, typename G>
    static void Push(const A& action, G& down, SegmentTreeIterator)
        requires(requires { action.Push(down); })
    {
        action.Push(down);
    }

    template <typename A, typename... Ts>
    static void Push(const A& action, MonoidProduct<Ts...>& ms, SegmentTreeIterator iter) {
        std::apply([&](auto&&... items) { (Push(action, items, iter), ...); }, ms);
    }

    template <typename S>
    static void SelfPush(S& scheme, SegmentTreeIterator iter)
        requires(requires { scheme.SelfPush(iter); })
    {
        scheme.SelfPush(iter);
    }

    template <typename S>
    static void SelfPush(S& scheme, SegmentTreeIterator)
        requires(requires { scheme.SelfPush(); })
    {
        scheme.SelfPush();
    }
};

template <typename A, typename G>
concept Respects = requires(const A& a, G& g, SegmentTreeIterator i) { PushTraits::Push(a, g, i); };

template <typename A, typename M>
concept Action = Monoid<M> && Respects<A, M> && Respects<A, A>;

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_SCHEME_CONCEPTS_ACTION_HPP_