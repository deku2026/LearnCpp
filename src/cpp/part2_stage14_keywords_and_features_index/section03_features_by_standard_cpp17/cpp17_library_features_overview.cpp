// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section03_features_by_standard_cpp17
// Item     : cpp17_library_features_overview
// Topic id : part2/stage14/section03/cpp17_library_features_overview
//
// Covers: C++17 library feature sampler

#include "learn/topic_registry.hpp"

#include <any>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace {

void demo_basics() {
    std::optional<int> o = 10;
    LEARN_CHECK(o.has_value());
    LEARN_CHECK(*o == 10);
}

void demo_intermediate() {
    std::variant<int, std::string> v = 3;
    LEARN_CHECK(std::get<int>(v) == 3);
    v = std::string{"hi"};
    LEARN_CHECK(std::get<std::string>(v) == "hi");
}

void demo_expert() {
    std::any a = 5;
    LEARN_CHECK(std::any_cast<int>(a) == 5);
    std::string_view sv{"abc"};
    LEARN_CHECK(sv.substr(1) == "bc");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section03/cpp17_library_features_overview", run>;

}  // namespace
