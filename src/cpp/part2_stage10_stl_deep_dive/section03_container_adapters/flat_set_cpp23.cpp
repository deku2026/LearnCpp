// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_set_cpp23
// Topic id : part2/stage10/section03_container_adapters/flat_set_cpp23
//
// Covers: C++23 std::flat_set ordered vector-based set adapter

#include "learn/topic_registry.hpp"

#include <cassert>
#include <version>

#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
#include <flat_set>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    std::flat_set<int> fs{3, 1, 4, 1, 5};
    assert(fs.size() == 4);
    assert(*fs.begin() == 1);
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    std::flat_set<int> fs;
    fs.insert(2);
    fs.insert(1);
    fs.insert(2);
    assert(fs.size() == 2);
    assert(fs.contains(1));
    fs.erase(1);
    assert(!fs.contains(1));
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    std::flat_set<int> fs{1, 2, 3, 4, 5};
    auto it = fs.lower_bound(3);
    assert(*it == 3);
    assert(fs.find(9) == fs.end());
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03_container_adapters/flat_set_cpp23", run>;

}  // namespace
