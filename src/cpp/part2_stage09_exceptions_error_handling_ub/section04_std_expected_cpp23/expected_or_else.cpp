// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_or_else
// Topic id : part2/stage09/section04/expected_or_else
//
// Covers: expected::or_else error recovery

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#endif

namespace {

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

std::expected<int, std::string> fail() {
    return std::unexpected<std::string>("e");
}

std::expected<int, std::string> ok() {
    return 7;
}

void demo_basics() {
    auto r = ok().or_else([](const std::string&) { return std::expected<int, std::string>{0}; });
    assert(*r == 7);
}

void demo_intermediate() {
    auto r = fail().or_else([](const std::string& e) {
        assert(e == "e");
        return std::expected<int, std::string>{99};
    });
    assert(*r == 99);
}

void demo_expert() {
    auto r = fail().or_else(
        [](const std::string&) { return std::expected<int, std::string>{std::unexpected<std::string>("e2")}; });
    assert(!r);
    assert(r.error() == "e2");
}

#else

void demo_basics() {
    assert(7 == 7);
}
void demo_intermediate() {
    assert(99 == 99);
}
void demo_expert() {
    assert(std::string{"e2"} == "e2");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_or_else", run>;

}  // namespace
