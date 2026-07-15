// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : container_choice_cache_complexity
// Topic id : part2/stage10/section01_sequence_containers/container_choice_cache_complexity
//
// Covers: choose container by complexity + cache locality; prefer vector

#include "learn/topic_registry.hpp"

#include <cassert>
#include <list>
#include <numeric>
#include <vector>

namespace {

void demo_basics() {
    // Default choice: contiguous vector for most workloads
    std::vector<int> v(1000);
    std::iota(v.begin(), v.end(), 0);
    long long sum = 0;
    for (int x : v) {
        sum += x;
    }
    assert(sum == 499500);
}

void demo_intermediate() {
    // list: O(1) splice/insert given iterator, but poor locality
    std::list<int> L{1, 2, 3};
    auto it = std::next(L.begin());
    L.insert(it, 99);
    assert(*std::next(L.begin()) == 99);
    // deque: efficient front + back
    // array: fixed compile-time size
    // Prefer vector unless measured need for list/deque properties
    assert(L.size() == 4);
}

void demo_expert() {
    constexpr int N = 5000;
    std::vector<int> v(N);
    std::list<int> L(N);
    std::iota(v.begin(), v.end(), 1);
    std::iota(L.begin(), L.end(), 1);

    auto sum_vec = [&] {
        long long s = 0;
        for (int x : v) {
            s += x;
        }
        return s;
    };
    auto sum_list = [&] {
        long long s = 0;
        for (int x : L) {
            s += x;
        }
        return s;
    };
    assert(sum_vec() == sum_list());
    // Both correct; vector usually wins on modern CPUs due to cache
    assert(v.size() == L.size());
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section01_sequence_containers/container_choice_cache_complexity", run>;

}  // namespace
