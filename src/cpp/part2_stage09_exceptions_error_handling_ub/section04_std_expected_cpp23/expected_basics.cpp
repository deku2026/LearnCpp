// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_basics
// Topic id : part2/stage09/section04/expected_basics
//
// Covers: std::expected basics (C++23) with portable fallback

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

std::expected<int, std::string> parse_positive(int x) {
    if (x <= 0) {
        return std::unexpected<std::string>("non-positive");
    }
    return x;
}

void demo_basics() {
    auto ok = parse_positive(5);
    LEARN_CHECK(ok.has_value());
    LEARN_CHECK(*ok == 5);
}

void demo_intermediate() {
    auto bad = parse_positive(0);
    LEARN_CHECK(!bad.has_value());
    LEARN_CHECK(bad.error() == "non-positive");
}

void demo_expert() {
    auto v = parse_positive(10).value_or(-1);
    LEARN_CHECK(v == 10);
    auto w = parse_positive(-1).value_or(-1);
    LEARN_CHECK(w == -1);
}

#else

// Portable mini-expected for toolchains without std::expected
template <class T, class E>
struct MiniExpected {
    bool ok = true;
    T value{};
    E error{};
    bool has_value() const { return ok; }
    const T& operator*() const { return value; }
    const E& err() const { return error; }
    T value_or(T d) const { return ok ? value : d; }
};

MiniExpected<int, std::string> parse_positive(int x) {
    if (x <= 0) {
        return {false, 0, "non-positive"};
    }
    return {true, x, {}};
}

void demo_basics() {
    auto ok = parse_positive(5);
    LEARN_CHECK(ok.has_value());
    LEARN_CHECK(*ok == 5);
}

void demo_intermediate() {
    auto bad = parse_positive(0);
    LEARN_CHECK(!bad.has_value());
    LEARN_CHECK(bad.err() == "non-positive");
}

void demo_expert() {
    LEARN_CHECK(parse_positive(10).value_or(-1) == 10);
    LEARN_CHECK(parse_positive(-1).value_or(-1) == -1);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/expected_basics", run>;

}  // namespace
