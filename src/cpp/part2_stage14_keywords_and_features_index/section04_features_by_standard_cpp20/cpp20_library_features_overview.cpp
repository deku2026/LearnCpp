// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section04_features_by_standard_cpp20
// Item     : cpp20_library_features_overview
// Topic id : part2/stage14/section04/cpp20_library_features_overview
//
// Covers: C++20 library feature sampler

#include "learn/topic_registry.hpp"

#include <memory>
#include <span>
#include <string>
#include <version>

namespace {

void demo_basics() {
    auto p = std::make_shared<int>(5);
    LEARN_CHECK(*p == 5);
    std::span<const int> empty{};
    LEARN_CHECK(empty.empty());
}

void demo_intermediate() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    // jthread present on this toolchain when macro is set
    LEARN_CHECK(__cpp_lib_jthread >= 201911L);
#else
    LEARN_CHECK(true);
#endif
#if defined(__cpp_lib_span) && __cpp_lib_span >= 202002L
    int a[2]{7, 8};
    std::span<int, 2> s{a};
    LEARN_CHECK(s[0] == 7);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_constexpr_algorithms)
    LEARN_CHECK(true);
#else
    LEARN_CHECK(true);
#endif
    std::string s = "cpp20";
    LEARN_CHECK(s.starts_with("cpp") || s.find("cpp") == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section04/cpp20_library_features_overview", run>;

}  // namespace
