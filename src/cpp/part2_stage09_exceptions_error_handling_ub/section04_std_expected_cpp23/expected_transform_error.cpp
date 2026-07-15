// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_transform_error
// Topic id : part2/stage09/section04/expected_transform_error
//
// Covers: expected::transform_error maps error channel

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#endif

namespace {

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

void demo_basics() {
    std::expected<int, std::string> e{std::unexpected<std::string>("bad")};
    auto r = e.transform_error([](const std::string& s) { return s + "!"; });
    LEARN_CHECK(!r);
    LEARN_CHECK(r.error() == "bad!");
}

void demo_intermediate() {
    std::expected<int, std::string> e{10};
    auto r = e.transform_error([](const std::string& s) { return s + "!"; });
    LEARN_CHECK(r.has_value());
    LEARN_CHECK(*r == 10);
}

void demo_expert() {
    std::expected<int, int> e{std::unexpected<int>(3)};
    auto r = e.transform_error([](int c) { return std::to_string(c); });
    LEARN_CHECK(!r);
    LEARN_CHECK(r.error() == "3");
}

#else

void demo_basics() {
    LEARN_CHECK(std::string{"bad"} + "!" == "bad!");
}
void demo_intermediate() {
    LEARN_CHECK(10 == 10);
}
void demo_expert() {
    LEARN_CHECK(std::to_string(3) == "3");
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_transform_error", run>;

}  // namespace
