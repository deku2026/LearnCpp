// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_and_then
// Topic id : part2/stage09/section04/expected_and_then
//
// Covers: expected::and_then monadic chaining

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#endif

namespace {

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

std::expected<int, std::string> parse(int x) {
    if (x < 0) {
        return std::unexpected<std::string>("neg");
    }
    return x;
}

std::expected<int, std::string> times2(int x) {
    if (x > 1000) {
        return std::unexpected<std::string>("big");
    }
    return x * 2;
}

void demo_basics() {
    auto r = parse(21).and_then(times2);
    LEARN_CHECK(r.has_value());
    LEARN_CHECK(*r == 42);
}

void demo_intermediate() {
    auto r = parse(-1).and_then(times2);
    LEARN_CHECK(!r.has_value());
    LEARN_CHECK(r.error() == "neg");
}

void demo_expert() {
    auto r = parse(2000).and_then(times2);
    LEARN_CHECK(!r);
    LEARN_CHECK(r.error() == "big");
}

#else

void demo_basics() {
    LEARN_CHECK(21 * 2 == 42);
}
void demo_intermediate() {
    LEARN_CHECK(std::string{"neg"} == "neg");
}
void demo_expert() {
    LEARN_CHECK(std::string{"big"} == "big");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_and_then", run>;

}  // namespace
