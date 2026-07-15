// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section06_features_by_standard_cpp23_library
// Item     : cpp23_library_feature_checklist
// Topic id : part2/stage14/section06/cpp23_library_feature_checklist
//
// Covers: C++23 library feature checklist samples

#include "learn/topic_registry.hpp"

#include <expected>
#include <optional>
#include <string>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    std::expected<int, int> e = 10;
    LEARN_CHECK(e.has_value());
    LEARN_CHECK(*e == 10);
#else
    std::optional<int> e = 10;
    LEARN_CHECK(e.has_value());
    LEARN_CHECK(*e == 10);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    std::string s = "hello";
    LEARN_CHECK(s.contains("ell"));
#else
    std::string s = "hello";
    LEARN_CHECK(s.find("ell") != std::string::npos);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    LEARN_CHECK(__cpp_lib_print >= 202207L);
#else
    LEARN_CHECK(true);
#endif
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    LEARN_CHECK(__cpp_lib_generator >= 202207L);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section06/cpp23_library_feature_checklist", run>;

}  // namespace
