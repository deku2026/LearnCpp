// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_multimap_cpp23
// Topic id : part2/stage10/section03_container_adapters/flat_multimap_cpp23
//
// Covers: C++23 std::flat_multimap ordered multi-key map adapter

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
#include <flat_map>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_multimap<std::string, int> fmm{{"a", 1}, {"a", 2}, {"b", 3}};
    LEARN_CHECK(fmm.size() == 3);
    LEARN_CHECK(fmm.count("a") == 2);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_multimap<int, char> fmm{{1, 'x'}, {1, 'y'}, {2, 'z'}};
    auto r = fmm.equal_range(1);
    int n = 0;
    for (auto it = r.first; it != r.second; ++it) {
        ++n;
    }
    LEARN_CHECK(n == 2);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    std::flat_multimap<int, int> fmm{{1, 10}, {1, 11}, {2, 20}};
    fmm.erase(1);
    LEARN_CHECK(fmm.size() == 1);
    LEARN_CHECK(fmm.begin()->second == 20);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03_container_adapters/flat_multimap_cpp23", run>;

}  // namespace
