#ifndef CODE_FOR_REUSE_GRAPH_FLOW_HPP_
#define CODE_FOR_REUSE_GRAPH_FLOW_HPP_

#include <compare>
#include <cstdint>
#include <deque>
#include <functional>

#include "../data_structures/used_counter.hpp"

using Cap = int64_t;
const Cap kMax = 4e18;

struct Flow {
    struct Vertex {
        friend struct Flow;

        Vertex() = default;

        std::strong_ordering operator<=>(const Vertex&) const = default;
        bool operator==(const Vertex&) const = default;

      private:
        explicit Vertex(int v) : v(v) {
        }

        int v = -1;
    };

    struct Edge {
        friend struct Flow;

        Edge() = default;

        std::strong_ordering operator<=>(const Edge&) const = default;
        bool operator==(const Edge&) const = default;

      private:
        explicit Edge(int e) : e(e) {
        }

        int e = -1;
    };

    struct FlowUsedCounter : private UsedCounter {
        using UsedCounter::Assign;
        using UsedCounter::Reset;
        using UsedCounter::UsedCounter;

        bool operator[](Vertex v) const {
            return UsedCounter::operator[](v.v);
        }

        int Get(Vertex v) const {
            return UsedCounter::Get(v.v);
        }

        void Set(Vertex v, int x) {
            return UsedCounter::Set(v.v, x);
        }

        void Use(Vertex v) {
            return UsedCounter::Use(v.v);
        }
    };

    struct EdgeData {
        Vertex to;
        Cap cap;
    };

    struct Graph : private std::vector<std::vector<Edge>> {
        using vector::vector;

        explicit Graph(const vector& g) : vector(g) {
        }

        Edge Last(Vertex v) const {
            return (*this)[v.v].back();
        }

        void Pop(Vertex v) {
            (*this)[v.v].pop_back();
        }

        void Add(Vertex v, Edge e) {
            (*this)[v.v].push_back(e);
        }

        bool HasEdges(Vertex v) const {
            return !(*this)[v.v].empty();
        }
    };

    const Vertex S{0};
    const Vertex T{1};

    explicit Flow(int approx_vertices = 0, int approx_edges = 0) {
        g.reserve(approx_vertices);
        edges.reserve(approx_edges * 2);
        g.resize(2);
    }

    Vertex AddVertex() {
        Vertex ans{Vertices()};
        g.emplace_back();
        return ans;
    }

    EdgeData& At(Edge e) {
        return edges[e.e];
    }

    const EdgeData& At(Edge e) const {
        return edges[e.e];
    }

    Edge AddEdge(Vertex from, Vertex to, Cap cap) {
        Edge to_e{Edges()};
        edges.emplace_back(to, cap);
        edges.emplace_back(from, 0);
        g[from.v].push_back(to_e);
        g[to.v].push_back(Backward(to_e));
        return to_e;
    }

    Cap CanPush(Edge edge) const {
        return At(edge).cap;
    }

    Cap Flowing(Edge edge) const {
        return At(Backward(edge)).cap;
    }

    Cap Capacity(Edge edge) const {
        return CanPush(edge) + Flowing(edge);
    }

    Vertex To(Edge edge) const {
        return At(edge).to;
    }

    void AddFlow(Edge edge, Cap cnt) {
        At(edge).cap -= cnt;
        At(Backward(edge)).cap += cnt;
    }

    Cap MaxCapacity() const {
        Cap mx = 0;
        for (int i = 0; i < Edges(); i += 2) {
            mx = std::max(mx, Capacity(Edge{i}));
        }
        return mx;
    }

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
        std::vector<int> nxt_(Vertices());
        std::vector<int> h_(Vertices());
        std::vector<Cap> extra_(Vertices());

        auto h = [&](Vertex v) -> int& { return h_[v.v]; };

        auto nxt = [&](Vertex v) -> int& { return nxt_[v.v]; };

        auto extra = [&](Vertex v) -> Cap& { return extra_[v.v]; };

        auto push = [&](Vertex v, Edge e) {
            Cap val = std::min(extra(v), CanPush(e));
            AddFlow(e, val);
            extra(v) -= val;
            extra(To(e)) += val;
        };

        auto relabel = [&](Vertex v) {
            int mn = Vertices() + 10;
            for (auto i : Edges(v)) {
                if (CanPush(i) > 0) {
                    mn = std::min(mn, h(To(i)));
                }
            }
            h(v) = mn + 1;
        };

        auto discharge = [&](Vertex v) {
            while (extra(v) > 0) {
                Edge i = Edges(v)[nxt(v)];
                if (CanPush(i) > 0 && h(To(i)) < h(v)) {
                    push(v, i);
                }
                if (extra(v)) {
                    ++nxt(v);
                    if (nxt(v) == Edges(v).size()) {
                        nxt(v) = 0;
                        relabel(v);
                    }
                }
            }
        };

        for (auto i : Edges(S)) {
            Cap c = CanPush(i);
            AddFlow(i, c);
            extra(To(i)) += c;
        }

        h(S) = Vertices();
        for (int f = 1; f;) {
            f = 0;
            for (int i = 0; i < Vertices(); ++i) {
                Vertex v{i};
                if (extra(v) && v != T && v != S) {
                    f = 1;
                    discharge(v);
                }
            }
        }

        return extra(T);
    }

    int Vertices() const {
        return g.size();
    }

    int Edges() const {
        return edges.size();
    }

    std::vector<Edge> Cut() {
        used.Assign(Vertices());
        std::vector<Edge> ans;
        FrontCutDFS(S);
        for (int i = 0; i < Edges(); i += 2) {
            Edge e{i};
            if (!used[To(e)] && used[To(Backward(e))]) {
                ans.push_back(e);
            }
        }
        return ans;
    }

    std::vector<Edge>& Edges(Vertex v) {
        return g[v.v];
    }

  private:
    void FrontCutDFS(Vertex v) {
        used.Use(v);
        for (auto u : Edges(v)) {
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
        while (gr.HasEdges(v)) {
            auto i = gr.Last(v);
            if (!used[To(i)] && CanPush(i) >= at_least) {
                Cap flow = DFS(To(i), at_least, std::min(prefix, CanPush(i)), gr);
                if (flow > 0) {
                    AddFlow(i, flow);
                    return flow;
                }
            }
            gr.Pop(v);
        }
        return 0;
    }

    Graph DinicBFS(Cap at_least, bool& fail) {
        Graph new_g(Vertices());
        used.Assign(Vertices());
        used.Set(S, 1);
        std::deque<Vertex> q = {S};
        while (!q.empty()) {
            Vertex v = q.front();
            q.pop_front();
            for (auto i : Edges(v)) {
                if (CanPush(i) < at_least) {
                    continue;
                }
                if (!used[To(i)]) {
                    used.Set(To(i), used.Get(v) + 1);
                    q.push_back(To(i));
                }
                if (used.Get(To(i)) == used.Get(v) + 1) {
                    new_g.Add(v, i);
                }
            }
        }
        if (used.Get(T) == 0) {
            fail = true;
        }
        return new_g;
    }

    static Edge Backward(Edge edge) {
        return Edge{edge.e ^ 1};
    }

  private:
    std::vector<std::vector<Edge>> g;
    std::vector<EdgeData> edges;
    FlowUsedCounter used;
};

#endif  // CODE_FOR_REUSE_GRAPH_FLOW_HPP_