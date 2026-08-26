#ifndef CODE_FOR_REUSE_GRAPH_FLOW_COMMON_HPP_
#define CODE_FOR_REUSE_GRAPH_FLOW_COMMON_HPP_

#include <compare>

#include "data_structures/used_counter.hpp"

struct Vertex {
    friend struct Flow;
    friend struct MCMF;
    friend struct FlowUsedCounter;

    template <typename>
    friend struct GraphHolder;
    template <typename>
    friend struct VertexContainer;

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
    friend struct MCMF;

    template <typename>
    friend struct EdgeHolder;
    template <typename>
    friend struct GraphHolder;
    template <typename>
    friend struct EdgeContainer;

    Edge() = default;

    std::strong_ordering operator<=>(const Edge&) const = default;
    bool operator==(const Edge&) const = default;

  private:
    explicit Edge(int e) : e(e) {
    }

    int e = -1;
};

template <typename T>
struct VertexContainer : std::vector<T> {
    using std::vector<T>::vector;

    decltype(auto) operator[](Vertex v) {
        return std::vector<T>::operator[](v.v);
    }

    decltype(auto) operator[](Vertex v) const {
        return std::vector<T>::operator[](v.v);
    }
};

template <typename T>
struct EdgeContainer : std::vector<T> {
    using std::vector<T>::vector;

    decltype(auto) operator[](Edge e) {
        return std::vector<T>::operator[](e.e);
    }

    decltype(auto) operator[](Edge e) const {
        return std::vector<T>::operator[](e.e);
    }
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

template <typename EdgeData>
struct EdgeHolder {
    auto CanPush(Edge edge) const {
        return edges[edge].cap;
    }

    auto Flowing(Edge edge) const {
        return edges[Backward(edge)].cap;
    }

    auto Capacity(Edge edge) const {
        return CanPush(edge) + Flowing(edge);
    }

    auto Cost(Edge edge) const {
        return edges[edge].cost;
    }

    Vertex To(Edge edge) const {
        return edges[edge].to;
    }

    Vertex From(Edge edge) const {
        return To(Backward(edge));
    }

    void AddFlow(Edge edge, auto cnt) {
        edges[edge].cap -= cnt;
        edges[Backward(edge)].cap += cnt;
    }

    int Edges() const {
        return edges.size();
    }

    auto MaxCapacity() const {
        decltype(Capacity(std::declval<Edge>())) mx = 0;
        for (int i = 0; i < Edges(); i += 2) {
            mx = std::max(mx, Capacity(Edge{i}));
        }
        return mx;
    }

    static Edge Backward(Edge edge) {
        return Edge{edge.e ^ 1};
    }

  protected:
    EdgeContainer<EdgeData> edges;
};

template <typename EdgeData>
struct GraphHolder : EdgeHolder<EdgeData> {
    using Base = EdgeHolder<EdgeData>;
    using Base::Backward;
    using Base::Edges;
    using Base::edges;

    explicit GraphHolder(int approx_vertices = 0, int approx_edges = 0) {
        g.reserve(approx_vertices);
        edges.reserve(approx_edges * 2);
        g.resize(2);
    }

    Vertex AddVertex() {
        Vertex ans{Vertices()};
        g.emplace_back();
        return ans;
    }

    Edge AddEdge(Vertex from, Vertex to, auto cap) {
        Edge to_e{Edges()};
        edges.emplace_back(to, cap);
        edges.emplace_back(from, 0);
        g[from.v].emplace_back(to_e);
        g[to.v].emplace_back(Backward(to_e));
        return to_e;
    }

    Edge AddEdge(Vertex from, Vertex to, auto cap, auto cost) {
        Edge to_e{Edges()};
        edges.emplace_back(to, cap, cost);
        edges.emplace_back(from, 0, -cost);
        g[from.v].emplace_back(to_e);
        g[to.v].emplace_back(Backward(to_e));
        return to_e;
    }

    int Vertices() const {
        return g.size();
    }

    std::vector<Edge>& EdgesFrom(Vertex v) {
        return g[v.v];
    }

    Vertex S{0};
    Vertex T{1};

  protected:
    std::vector<std::vector<Edge>> g;
};

#endif  // CODE_FOR_REUSE_GRAPH_FLOW_COMMON_HPP_