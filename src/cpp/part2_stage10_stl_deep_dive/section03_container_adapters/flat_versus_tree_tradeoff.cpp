// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_versus_tree_tradeoff
// Topic id : part2/stage10/section03_container_adapters/flat_versus_tree_tradeoff
//
// Covers: flat_map vs map: cache-friendly lookup/traversal vs O(n) insert

#include "learn/topic_registry.hpp"

#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <version>

#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
#include <flat_map>
#endif

namespace {

void demo_basics() {
    // map: node-based, O(log n) insert, stable iterators, poorer locality
    std::map<int, int> m{{1, 1}, {2, 2}, {3, 3}};
    assert(m.find(2)->second == 2);
    // Prefer flat_map when mostly lookup/traverse after bulk build
    assert(m.size() == 3);
}

void demo_intermediate() {
    std::map<std::string, int> tree{{"b", 2}, {"a", 1}, {"c", 3}};
    int sum = 0;
    for (const auto& [k, v] : tree) {
        (void)k;
        sum += v;
    }
    assert(sum == 6);
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_map<std::string, int> flat{{"b", 2}, {"a", 1}, {"c", 3}};
    int fsum = 0;
    for (const auto& [k, v] : flat) {
        (void)k;
        fsum += v;
    }
    assert(fsum == 6);
#endif
}

void demo_expert() {
    // Tradeoff summary (behavioral demo):
    // - map insert: O(log n), no full iterator invalidation
    // - flat_map insert: O(n) shift, invalidates like vector
    // - flat_map find/traverse: better cache, often faster in practice
    std::map<int, int> m;
    for (int i = 0; i < 100; ++i) {
        m[i] = i;
    }
    assert(m.size() == 100);
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_map<int, int> fm;
    for (int i = 0; i < 100; ++i) {
        fm[i] = i;
    }
    assert(fm.size() == 100);
    assert(fm.find(50)->second == 50);
#endif
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
    ::learn::topic<"part2/stage10/section03_container_adapters/flat_versus_tree_tradeoff", run>;

}  // namespace
