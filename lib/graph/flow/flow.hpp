#ifndef CODE_FOR_REUSE_GRAPH_FLOW_FLOW_HPP_
#define CODE_FOR_REUSE_GRAPH_FLOW_FLOW_HPP_

#include <deque>
#include <cstdint>

#include "common.hpp"

using Cap = int64_t;
const Cap kMax = 4e18;

struct FlowEdgeData {
    Vertex to;
    Cap cap;
};

struct Flow : GraphHolder<FlowEdgeData> {
    using GraphHolder::GraphHolder;
    using Graph = VertexContainer<std::vector<Edge>>;

    Cap Dinic(Cap at_least = 1) {
        Cap ans = 0;
        bool fail = false;
        Graph gr = DinicBFS(at_least, fail);
        while (!fail) {
            Cap x = DinicStep(at_least, gr);
            if (x == 0) {
                break;
            }
            ans += x;
            gr = DinicBFS(at_least, fail);
        }
        return ans;
    }

    Cap ScalableDinic() {
        Cap x = MaxCapacity();
        Cap ans = 0;
        while (x > 0) {
            ans += Dinic(x);
            x /= 2;
        }
        return ans;
    }

    Cap PushRelabel() {
        VertexContainer<int> nxt(Vertices());
        VertexContainer<int> h(Vertices());
        VertexContainer<Cap> extra(Vertices());

        auto push = [&](Vertex v, Edge e) {
            Cap val = std::min(extra[v], CanPush(e));
            AddFlow(e, val);
            extra[v] -= val;
            extra[To(e)] += val;
        };

        auto relabel = [&](Vertex v) {
            int mn = Vertices() + 10;
            for (auto i : EdgesFrom(v)) {
                if (CanPush(i) > 0) {
                    mn = std::min(mn, h[To(i)]);
                }
            }
            h[v] = mn + 1;
        };

        auto discharge = [&](Vertex v) {
            while (extra[v] > 0) {
                Edge i = EdgesFrom(v)[nxt[v]];
                if (CanPush(i) > 0 && h[To(i)] < h[v]) {
                    push(v, i);
                }
                if (extra[v]) {
                    ++nxt[v];
                    if (nxt[v] == EdgesFrom(v).size()) {
                        nxt[v] = 0;
                        relabel(v);
                    }
                }
            }
        };

        for (auto i : EdgesFrom(S)) {
            Cap c = CanPush(i);
            AddFlow(i, c);
            extra[To(i)] += c;
        }

        h[S] = Vertices();
        for (int f = 1; f;) {
            f = 0;
            for (int i = 0; i < Vertices(); ++i) {
                Vertex v{i};
                if (extra[v] > 0 && v != T && v != S) {
                    f = 1;
                    discharge(v);
                }
            }
        }

        return extra[T];
    }

    std::vector<Edge> Cut() {
        used.Assign(Vertices());
        std::vector<Edge> ans;
        FrontCutDFS(S);
        for (int i = 0; i < Edges(); i += 2) {
            Edge e{i};
            if (!used[To(e)] && used[From(e)]) {
                ans.push_back(e);
            }
        }
        return ans;
    }

  private:
    void FrontCutDFS(Vertex v) {
        used.Use(v);
        for (auto u : EdgesFrom(v)) {
            if (!used[To(u)] && CanPush(u) > 0) {
                FrontCutDFS(To(u));
            }
        }
    }

    Cap DinicStep(Cap at_least, Graph& gr) {
        Cap ans = 0;
        used.Assign(Vertices());
        while (Cap x = DFS(S, at_least, kMax, gr)) {
            used.Reset();
            ans += x;
        }
        return ans;
    }

    Cap DFS(Vertex v, Cap at_least, Cap prefix, Graph& gr) {
        if (v == T) {
            return prefix;
        }
        used.Use(v);
        while (!gr[v].empty()) {
            auto i = gr[v].back();
            if (!used[To(i)] && CanPush(i) >= at_least) {
                Cap flow = DFS(To(i), at_least, std::min(prefix, CanPush(i)), gr);
                if (flow > 0) {
                    AddFlow(i, flow);
                    return flow;
                }
            }
            gr[v].pop_back();
        }
        return 0;
    }

    Graph DinicBFS(Cap at_least, bool& fail) {
        Graph new_g(Vertices());
        used.Assign(Vertices());
        used.Set(S, 1);
        std::deque q = {S};
        while (!q.empty()) {
            Vertex v = q.front();
            q.pop_front();
            for (auto i : EdgesFrom(v)) {
                if (CanPush(i) < at_least) {
                    continue;
                }
                if (!used[To(i)]) {
                    used.Set(To(i), used.Get(v) + 1);
                    q.push_back(To(i));
                }
                if (used.Get(To(i)) == used.Get(v) + 1) {
                    new_g[v].push_back(i);
                }
            }
        }
        if (used.Get(T) == 0) {
            fail = true;
        }
        return new_g;
    }

  private:
    FlowUsedCounter used;
};

#endif  // CODE_FOR_REUSE_GRAPH_FLOW_FLOW_HPP_