// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section02_sfinae_deep
// Item     : enable_if_deep_usage
// Topic id : part6/e/section02/enable_if_deep_usage

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <string_view>
#include <type_traits>

namespace {

template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
[[nodiscard]] constexpr std::string_view numeric_family(T) noexcept {
    return "integral";
}

template <class T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
[[nodiscard]] constexpr std::string_view numeric_family(T) noexcept {
    return "floating";
}

template <class T>
using enable_enum_t = std::enable_if_t<std::is_enum_v<T>, std::underlying_type_t<T>>;

template <class T>
[[nodiscard]] constexpr auto enum_value(T value) noexcept -> enable_enum_t<T> {
    return static_cast<enable_enum_t<T>>(value);
}

enum class Mode : unsigned char { fast = 3 };

template <class T>
concept HasNumericFamily = requires(T value) { numeric_family(value); };

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section02/enable_if_deep_usage"};

    static_assert(numeric_family(1) == "integral");
    static_assert(numeric_family(1.0) == "floating");
    static_assert(HasNumericFamily<int>);
    static_assert(!HasNumericFamily<void*>);
    static_assert(enum_value(Mode::fast) == 3);
    LEARN_EXPECT_EQ(checks, numeric_family(42U), std::string_view{"integral"});
    LEARN_EXPECT_EQ(checks, enum_value(Mode::fast), static_cast<unsigned char>(3));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section02/enable_if_deep_usage", run>;

}  // namespace
