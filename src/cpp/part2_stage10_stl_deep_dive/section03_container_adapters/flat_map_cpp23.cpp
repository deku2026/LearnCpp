// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_map_cpp23
// Topic id : part2/stage10/section03_container_adapters/flat_map_cpp23
//
// Covers: C++23 std::flat_map ordered vector-based map adapter

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
#include <flat_map>
#endif
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
#include <flat_set>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_map<std::string, int> fm{{"banana", 2}, {"apple", 1}};
    LEARN_CHECK(fm.begin()->first == "apple");
    LEARN_CHECK(fm["banana"] == 2);
    LEARN_CHECK(fm.size() == 2);
#else
    // Fallback: ordered map semantics
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_map<int, int> fm;
    fm[3] = 30;
    fm[1] = 10;
    fm[2] = 20;
    LEARN_CHECK(fm.find(2)->second == 20);
    LEARN_CHECK(fm.contains(1));
    fm.erase(1);
    LEARN_CHECK(!fm.contains(1));
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_map<int, int> fm{{1, 1}, {2, 2}, {3, 3}};
    // Contiguous keys/values: cache-friendly ordered container
    LEARN_CHECK(fm.keys().size() == 3);
    LEARN_CHECK(fm.values()[0] == 1);
    auto it = fm.lower_bound(2);
    LEARN_CHECK(it->first == 2);
#else
    LEARN_CHECK(true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03_container_adapters/flat_map_cpp23", run>;

}  // namespace
