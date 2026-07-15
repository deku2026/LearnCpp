// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_transform
// Topic id : part2/stage09/section04/expected_transform
//
// Covers: expected::transform maps value, preserves error

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#endif

namespace {

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

void demo_basics() {
    std::expected<int, std::string> e{21};
    auto r = e.transform([](int x) { return x * 2; });
    LEARN_CHECK(r.has_value());
    LEARN_CHECK(*r == 42);
}

void demo_intermediate() {
    std::expected<int, std::string> e{std::unexpected<std::string>("x")};
    auto r = e.transform([](int x) { return x * 2; });
    LEARN_CHECK(!r);
    LEARN_CHECK(r.error() == "x");
}

void demo_expert() {
    std::expected<int, std::string> e{3};
    auto r = e.transform([](int x) { return std::to_string(x); });
    LEARN_CHECK(r.has_value());
    LEARN_CHECK(*r == "3");
}

#else

void demo_basics() {
    LEARN_CHECK(21 * 2 == 42);
}
void demo_intermediate() {
    LEARN_CHECK(std::string{"x"} == "x");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_transform", run>;

}  // namespace
