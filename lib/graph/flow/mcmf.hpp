#ifndef CODE_FOR_REUSE_GRAPH_FLOW_MCMF_HPP_
#define CODE_FOR_REUSE_GRAPH_FLOW_MCMF_HPP_

#include <cassert>
#include <queue>

#include "common.hpp"

struct MCMFEdgeData {
    Vertex to;
    int64_t cap;
    int64_t cost;
};

struct MCMF : GraphHolder<MCMFEdgeData> {
    using GraphHolder::GraphHolder;

    static constexpr int64_t kMax = 2e18;

    struct Result {
        int64_t total_flow = 0;
        int64_t total_cost = 0;
    };

    struct DJEntry {
        int64_t cost;
        Vertex v;

        bool operator==(const DJEntry&) const = default;
        std::strong_ordering operator<=>(const DJEntry&) const = default;
    };

    bool AreCostsNonNegative() const {
        for (int i = 0; i < Edges(); i += 2) {
            Edge e{i};
            if (Cost(e) < 0) {
                return false;
            }
        }
        return true;
    }

    void Reset() {
        phi.resize(Vertices());
        pr.resize(Vertices());
        dist.assign(Vertices(), kMax);
        dist[S] = 0;
    }

    void Ford() {
        Reset();
        for (int f = 1, cnt = 0; f; ++cnt) {
            f = 0;
            for (int e = 0; e < Edges(); ++e) {
                if (Edge idx{e}; CanPush(idx) && UpdateEdge(idx)) {
                    f = 1;
                }
            }
            assert(cnt < Vertices() + 10);
        }
    }

    void Dijkstra() {
        Reset();
        std::priority_queue<DJEntry, std::vector<DJEntry>, std::greater<>> pq;
        pq.emplace(0, S);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();
            if (d != dist[v]) continue;
            for (auto idx : EdgesFrom(v)) {
                if (CanPush(idx) && UpdateEdge(idx)) {
                    pq.emplace(dist[To(idx)], To(idx));
                }
            }
        }
    }

    void SquareDijkstra() {
        Reset();
        VertexContainer<bool> is_blocked(Vertices());
        while (!is_blocked[T]) {
            Vertex best_v;
            for (int i = 0; i < Vertices(); ++i) {
                Vertex v{i};
                if (!is_blocked[v] && dist[v] != kMax && (best_v == Vertex{} || dist[v] < dist[best_v])) {
                    best_v = v;
                }
            }
            if (best_v == Vertex{}) {
                return;
            }
            is_blocked[best_v] = true;
            for (auto e : EdgesFrom(best_v)) {
                UpdateEdge(e);
            }
        }
    }

    bool Retract(Result& result) {
        if (dist[T] == kMax) return false;
        int64_t min_cap = kMax;
        Vertex cur = T;
        while (cur != S) {
            min_cap = std::min(min_cap, CanPush(pr[cur]));
            cur = From(pr[cur]);
        }
        cur = T;
        while (cur != S) {
            AddFlow(pr[cur], min_cap);
            result.total_cost += min_cap * Cost(pr[cur]);
            cur = From(pr[cur]);
        }
        result.total_flow += min_cap;
        for (int i = 0; i < Vertices(); ++i) {
            Vertex v{i};
            if (phi[v] < kMax) {
                phi[v] += dist[v];
            }
        }
        return true;
    }

    // NB: No negative weight cycles are allowed
    Result Run() {
        Result result;
        if (!AreCostsNonNegative()) {
            Ford();
            if (!Retract(result)) {
                return result;
            }
        }
        while (true) {
            Dijkstra();
            if (!Retract(result)) {
                return result;
            }
        }
    }

    Result Hungarian() {
        Result result;
        if (!AreCostsNonNegative()) {
            Ford();
            if (!Retract(result)) {
                return result;
            }
        }
        while (true) {
            SquareDijkstra();
            if (!Retract(result)) {
                return result;
            }
        }
    }

  private:
    bool UpdateEdge(Edge e) {
        Vertex to = To(e);
        int64_t d = dist[From(e)];
        int64_t w = Cost(e) + phi[From(e)] - phi[to];
        if (dist[to] > d + w) {
            dist[to] = d + w;
            pr[to] = e;
            return true;
        }
        return false;
    }

  private:
    VertexContainer<Edge> pr;
    VertexContainer<int64_t> dist;
    VertexContainer<int64_t> phi;
};

#endif  // CODE_FOR_REUSE_GRAPH_FLOW_MCMF_HPP_