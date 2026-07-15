// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : optional_and_then_transform_or_else_cpp23
// Topic id : part2/stage09/section05/optional_and_then_transform_or_else_cpp23
//
// Covers: optional monadic ops (C++23) with portable fallback

#include "learn/topic_registry.hpp"

#include <optional>
#include <string>
#include <version>

namespace {

void demo_basics() {
    std::optional<int> o = 21;
#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L
    auto r = o.transform([](int x) { return x * 2; });
    LEARN_CHECK(r.has_value());
    LEARN_CHECK(*r == 42);
#else
    auto r = o.has_value() ? std::optional<int>{*o * 2} : std::nullopt;
    LEARN_CHECK(r && *r == 42);
#endif
}

void demo_intermediate() {
    std::optional<int> o = 5;
#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L
    auto r = o.and_then([](int x) -> std::optional<int> {
        if (x > 0) {
            return x + 1;
        }
        return std::nullopt;
    });
    LEARN_CHECK(r.value() == 6);
#else
    LEARN_CHECK(o.value() + 1 == 6);
#endif
}

void demo_expert() {
    std::optional<int> empty;
#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L
    auto r = empty.or_else([] { return std::optional<int>{99}; });
    LEARN_CHECK(*r == 99);
    auto t = empty.transform([](int x) { return x; });
    LEARN_CHECK(!t);
#else
    LEARN_CHECK(empty.value_or(99) == 99);
#endif
    (void)std::string{"ok"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage09/section05/optional_and_then_transform_or_else_cpp23", run>;

}  // namespace
