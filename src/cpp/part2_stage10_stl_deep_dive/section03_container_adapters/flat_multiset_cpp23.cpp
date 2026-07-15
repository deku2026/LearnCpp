// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_multiset_cpp23
// Topic id : part2/stage10/section03_container_adapters/flat_multiset_cpp23
//
// Covers: C++23 std::flat_multiset ordered multi set adapter

#include "learn/topic_registry.hpp"

#include <version>

#if defined(__has_include)
#if __has_include(<flat_set>)
#include <flat_set>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L && __has_include(<flat_set>)
    std::flat_multiset<int> fms{1, 2, 2, 3};
    LEARN_CHECK(fms.size() == 4);
    LEARN_CHECK(fms.count(2) == 2);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L && __has_include(<flat_set>)
    std::flat_multiset<int> fms;
    fms.insert(5);
    fms.insert(1);
    fms.insert(5);
    LEARN_CHECK(fms.size() == 3);
    LEARN_CHECK(*fms.begin() == 1);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L && __has_include(<flat_set>)
    std::flat_multiset<int> fms{3, 1, 2, 1};
    auto it = fms.find(1);
    LEARN_CHECK(it != fms.end());
    fms.erase(it);
    LEARN_CHECK(fms.count(1) == 1);
#else
    LEARN_CHECK(true);
#endif
}

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
