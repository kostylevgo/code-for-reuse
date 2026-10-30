#ifndef CODE_FOR_REUSE_DATA_STRUCTURES_RANDOM_HEAP_HPP_
#define CODE_FOR_REUSE_DATA_STRUCTURES_RANDOM_HEAP_HPP_

#include <cstdint>
#include <memory>
#include <random>
#include <utility>
#include <vector>

struct Seed {
    uint32_t seed;
};

template <class T>
struct RandomHeap : std::vector<T> {
    template <class... Args>
    explicit RandomHeap(Seed seed, Args&&... args) : gen(seed.seed), std::vector<T>(std::forward<Args>(args)...) {
    }

    T pop_random() {
        auto index = gen() % heap.size();
        T result = std::move(heap[index]);
        std::swap(heap[index], heap.back());
        heap.pop_back();
        return result;
    }

    std::mt19937 gen;
    std::vector<T> heap;
};

#endif  // CODE_FOR_REUSE_DATA_STRUCTURES_RANDOM_HEAP_HPP_