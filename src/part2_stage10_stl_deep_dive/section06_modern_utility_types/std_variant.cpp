// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_variant
// Topic id : part2/stage10/section06/std_variant

#ifdef __has_include
#if __has_include(<variant>)
#include <variant>
#define LEARN_HAS_VARIANT_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <string>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_variant";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_VARIANT_HEADER) && defined(__cpp_lib_variant) && __cpp_lib_variant >= 201606L
    learn::ExampleChecks checks{topic_id};
    using Value = std::variant<int, std::string, std::vector<int>>;

    Value value = 42;
    LEARN_EXPECT(checks, std::holds_alternative<int>(value));
    LEARN_EXPECT_EQ(checks, value.index(), 0U);
    LEARN_EXPECT_EQ(checks, std::get<int>(value), 42);

    value = std::string{"typed union"};
    const auto* text = std::get_if<std::string>(&value);
    LEARN_EXPECT(checks, text != nullptr);
    LEARN_EXPECT_EQ(checks, *text, "typed union");
    LEARN_EXPECT(checks, std::get_if<int>(&value) == nullptr);
    LEARN_EXPECT_THROWS(checks, std::bad_variant_access, std::get<int>(value));

    auto& numbers = value.emplace<std::vector<int>>(3, 7);
    numbers.push_back(9);
    LEARN_EXPECT_EQ(checks, (std::get<std::vector<int>>(value)), (std::vector{7, 7, 7, 9}));
    static_assert(std::variant_size_v<Value> == 3);
    static_assert(std::is_same_v<std::variant_alternative_t<1, Value>, std::string>);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 __cpp_lib_variant >= 201606L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_variant", run>;

}  // namespace
