// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_any
// Topic id : part2/stage10/section06/std_any

#ifdef __has_include
#if __has_include(<any>)
#include <any>
#define LEARN_HAS_ANY_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_any";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_ANY_HEADER) && defined(__cpp_lib_any) && __cpp_lib_any >= 201606L
    learn::ExampleChecks checks{topic_id};

    std::any payload = 42;
    LEARN_EXPECT(checks, payload.has_value());
    LEARN_EXPECT_EQ(checks, std::any_cast<int>(payload), 42);

    // Pointer-form any_cast is a non-throwing type probe.
    const int* number = std::any_cast<int>(&payload);
    const auto* wrong_type = std::any_cast<std::string>(&payload);
    LEARN_EXPECT(checks, number != nullptr);
    LEARN_EXPECT_EQ(checks, *number, 42);
    LEARN_EXPECT(checks, wrong_type == nullptr);

    auto& text = payload.emplace<std::string>(3, 'x');
    text += "-typed";
    LEARN_EXPECT_EQ(checks, std::any_cast<const std::string&>(payload), "xxx-typed");
    LEARN_EXPECT_THROWS(checks, std::bad_any_cast, std::any_cast<double>(payload));

    const std::any copied = payload;  // Contained values must be copy-constructible.
    LEARN_EXPECT_EQ(checks, std::any_cast<const std::string&>(copied), "xxx-typed");
    payload.reset();
    LEARN_EXPECT(checks, !payload.has_value());

    static_assert(std::is_copy_constructible_v<std::any>);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 __cpp_lib_any >= 201606L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_any", run>;

}  // namespace
