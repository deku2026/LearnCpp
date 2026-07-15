// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_as_rvalue_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/views_as_rvalue_cpp23
//
// Covers: views::as_rvalue C++23

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <ranges>
#include <string>
#include <utility>
#include <vector>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_ranges_as_rvalue) && __cpp_lib_ranges_as_rvalue >= 202207L
    std::vector<std::string> v{"a", "b"};
    std::vector<std::string> out;
    for (auto&& s : v | std::views::as_rvalue) {
        out.push_back(std::move(s));
    }
    assert(out.size() == 2);
    assert(out[0] == "a");
#else
    std::vector<std::string> v{"a", "b"};
    std::vector<std::string> out;
    for (auto& s : v) {
        out.push_back(std::move(s));
    }
    assert(out[0] == "a");
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges_as_rvalue) && __cpp_lib_ranges_as_rvalue >= 202207L
    std::vector<std::string> v{"x", "y", "z"};
    auto r = v | std::views::as_rvalue;
    static_assert(std::ranges::view<decltype(r)>);
    assert(std::ranges::distance(r) == 3);
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges_as_rvalue) && __cpp_lib_ranges_as_rvalue >= 202207L
    std::vector<std::string> v{"p", "q"};
    std::vector<std::string> dst;
    std::ranges::copy(v | std::views::as_rvalue, std::back_inserter(dst));
    assert(dst.size() == 2);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08_ranges_cpp23/views_as_rvalue_cpp23", run>;

}  // namespace
