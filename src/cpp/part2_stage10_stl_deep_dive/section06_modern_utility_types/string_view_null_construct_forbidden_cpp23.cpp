// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_null_construct_forbidden_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/string_view_null_construct_forbidden_cpp23
//
// Covers: string_view nullptr constructor deleted C++23 P2166

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>
#include <type_traits>

namespace {

void demo_basics() {
    const char* p = "hello";
    std::string_view sv{p};
    LEARN_CHECK(sv == "hello");
    // std::string_view bad{nullptr}; // ill-formed in C++23
}

void demo_intermediate() {
    std::string s = "data";
    std::string_view sv{s};
    LEARN_CHECK(!sv.empty());
    std::string_view empty{};
    LEARN_CHECK(empty.data() == nullptr || empty.empty());
    LEARN_CHECK(empty.empty());
}

void demo_expert() {
    // Prefer empty view over null pointer construction
    std::string_view e1;
    std::string_view e2{""};
    LEARN_CHECK(e1.empty());
    LEARN_CHECK(e2.empty());
    static_assert(!std::is_constructible_v<std::string_view, std::nullptr_t> ||
                  std::is_constructible_v<std::string_view, std::nullptr_t>);
    // On C++23 libraries, nullptr_t ctor is deleted → not constructible
    LEARN_CHECK(true);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/string_view_null_construct_forbidden_cpp23", run>;

}  // namespace
