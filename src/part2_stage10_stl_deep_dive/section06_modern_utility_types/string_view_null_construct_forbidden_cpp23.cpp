// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_null_construct_forbidden_cpp23
// Topic id : part2/stage10/section06/string_view_null_construct_forbidden_cpp23

#ifdef __has_include
#if __has_include(<string_view>)
#include <string_view>
#define LEARN_HAS_STRING_VIEW_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <cstddef>
#include <optional>
#include <type_traits>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/string_view_null_construct_forbidden_cpp23";

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L

// A nullable C pointer needs an explicit state check before constructing a view.
[[nodiscard]] std::optional<std::string_view> view_from_nullable(const char* pointer) noexcept {
    if (pointer == nullptr) {
        return std::nullopt;
    }
    return std::string_view{pointer};
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L
    learn::ExampleChecks checks{topic_id};
    const auto missing = view_from_nullable(nullptr);
    const auto present = view_from_nullable("safe");
    LEARN_EXPECT(checks, !missing.has_value());
    LEARN_EXPECT_EQ(checks, present.value_or("fallback"), "safe");

    // P2166 has no dedicated feature-test macro, so inspect the actual constructor set.
    constexpr bool nullptr_constructor_is_deleted = !std::is_constructible_v<std::string_view, std::nullptr_t>;
    if constexpr (nullptr_constructor_is_deleted) {
        static_assert(!std::is_constructible_v<std::string_view, std::nullptr_t>);
        return checks.result();
    } else {
        if (checks.result() != 0) {
            return checks.result();
        }
        return learn::ExampleChecks::unavailable(topic_id, "C++23 deleted string_view(nullptr_t) constructor");
    }
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 std::string_view");
#endif
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06/string_view_null_construct_forbidden_cpp23", run>;

}  // namespace
