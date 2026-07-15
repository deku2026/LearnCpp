// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_value_and_error
// Topic id : part2/stage09/section04/expected_value_and_error
//
// Covers: value(), error(), operator bool, value_or

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__has_include)
#if __has_include(<expected>)
#include <expected>
#endif
#endif
namespace {

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L && __has_include(<expected>)

std::expected<int, std::string> f(bool ok) {
    if (ok) {
        return 42;
    }
    return std::unexpected<std::string>("nope");
}

void demo_basics() {
    auto a = f(true);
    LEARN_CHECK(static_cast<bool>(a));
    LEARN_CHECK(a.value() == 42);
}

void demo_intermediate() {
    auto b = f(false);
    LEARN_CHECK(!b);
    LEARN_CHECK(b.error() == "nope");
    LEARN_CHECK(b.value_or(0) == 0);
}

void demo_expert() {
    auto a = f(true);
    LEARN_CHECK(*a == 42);
    auto b = f(false);
    // Prefer error()/value_or over value() when maybe empty.
    LEARN_CHECK(b.error().size() == 4);
}

#else

void demo_basics() {
    LEARN_CHECK(42 == 42);
}
void demo_intermediate() {
    LEARN_CHECK(std::string{"nope"} == "nope");
}
void demo_expert() {
    LEARN_CHECK(true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_value_and_error", run>;

}  // namespace
