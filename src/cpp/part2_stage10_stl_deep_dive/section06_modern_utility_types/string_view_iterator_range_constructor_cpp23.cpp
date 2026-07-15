// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_iterator_range_constructor_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/string_view_iterator_range_constructor_cpp23
//
// Covers: string_view iterator-pair / range constructor C++23 P1989

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>
#include <vector>
#include <version>

namespace {

void demo_basics() {
    std::string s = "hello";
    std::string_view sv{s.begin(), s.end()};
    assert(sv == "hello");
}

void demo_intermediate() {
    std::vector<char> chars{'H', 'i', '!'};
#if defined(__cpp_lib_containers_ranges) || (defined(__cplusplus) && __cplusplus >= 202302L)
    std::string_view sv(chars.begin(), chars.end());
    assert(sv == "Hi!");
#else
    std::string_view sv(chars.data(), chars.size());
    assert(sv == "Hi!");
#endif
}

void demo_expert() {
    const char buf[] = {'a', 'b', 'c', 'd'};
    std::string_view sv(buf, buf + 3);
    assert(sv == "abc");
    assert(sv.size() == 3);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/string_view_iterator_range_constructor_cpp23", run>;

}  // namespace
