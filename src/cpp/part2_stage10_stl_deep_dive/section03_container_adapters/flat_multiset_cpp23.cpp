// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_multiset_cpp23
// Topic id : part2/stage10/section03_container_adapters/flat_multiset_cpp23
//
// Covers: C++23 std::flat_multiset ordered multi set adapter

#include "learn/topic_registry.hpp"

#include <cassert>
#include <version>

#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
#include <flat_set>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    std::flat_multiset<int> fms{1, 2, 2, 3};
    assert(fms.size() == 4);
    assert(fms.count(2) == 2);
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    std::flat_multiset<int> fms;
    fms.insert(5);
    fms.insert(1);
    fms.insert(5);
    assert(fms.size() == 3);
    assert(*fms.begin() == 1);
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    std::flat_multiset<int> fms{3, 1, 2, 1};
    auto it = fms.find(1);
    assert(it != fms.end());
    fms.erase(it);
    assert(fms.count(1) == 1);
#else
    assert(true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03_container_adapters/flat_multiset_cpp23", run>;

}  // namespace
