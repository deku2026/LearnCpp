// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : railway_oriented_pipeline
// Topic id : part2/stage09/section04/railway_oriented_pipeline
//
// Covers: railway-oriented error pipeline with expected monads

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#endif

namespace {

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

using Ex = std::expected<int, std::string>;

Ex parse(const std::string& s) {
    if (s.empty()) {
        return std::unexpected<std::string>("empty");
    }
    int v = 0;
    for (char c : s) {
        if (c < '0' || c > '9') {
            return std::unexpected<std::string>("nan");
        }
        v = v * 10 + (c - '0');
    }
    return v;
}

Ex non_zero(int x) {
    if (x == 0) {
        return std::unexpected<std::string>("zero");
    }
    return x;
}

Ex times(int x) {
    return x * 2;
}

Ex pipeline(const std::string& s) {
    return parse(s).and_then(non_zero).and_then(times);
}

void demo_basics() {
    auto r = pipeline("21");
    LEARN_CHECK(r.has_value());
    LEARN_CHECK(*r == 42);
}

void demo_intermediate() {
    LEARN_CHECK(pipeline("").error() == "empty");
    LEARN_CHECK(pipeline("0").error() == "zero");
}

void demo_expert() {
    LEARN_CHECK(pipeline("x").error() == "nan");
    auto r = parse("5").transform([](int x) { return x + 1; }).and_then(non_zero);
    LEARN_CHECK(*r == 6);
}

#else

void demo_basics() {
    LEARN_CHECK(21 * 2 == 42);
}
void demo_intermediate() {
    LEARN_CHECK(std::string{"empty"} == "empty");
    LEARN_CHECK(std::string{"zero"} == "zero");
}
void demo_expert() {
    LEARN_CHECK(std::string{"nan"} == "nan");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/railway_oriented_pipeline", run>;

}  // namespace
