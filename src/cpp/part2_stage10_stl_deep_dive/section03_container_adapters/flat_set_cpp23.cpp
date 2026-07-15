// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_set_cpp23
// Topic id : part2/stage10/section03_container_adapters/flat_set_cpp23
//
// Covers: C++23 std::flat_set ordered vector-based set adapter

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
    std::flat_set<int> fs{3, 1, 4, 1, 5};
    LEARN_CHECK(fs.size() == 4);
    LEARN_CHECK(*fs.begin() == 1);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L && __has_include(<flat_set>)
    std::flat_set<int> fs;
    fs.insert(2);
    fs.insert(1);
    fs.insert(2);
    LEARN_CHECK(fs.size() == 2);
    LEARN_CHECK(fs.contains(1));
    fs.erase(1);
    LEARN_CHECK(!fs.contains(1));
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L && __has_include(<flat_set>)
    std::flat_set<int> fs{1, 2, 3, 4, 5};
    auto it = fs.lower_bound(3);
    LEARN_CHECK(*it == 3);
    LEARN_CHECK(fs.find(9) == fs.end());
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03_container_adapters/flat_set_cpp23", run>;

}  // namespace
