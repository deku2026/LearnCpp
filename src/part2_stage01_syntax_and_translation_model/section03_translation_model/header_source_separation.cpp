// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : header_source_separation
// Topic id : part2/stage01/section03/header_source_separation
// References: N4950 [basic.def], [basic.link], [cpp.include].

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

namespace temperature {

// Public header-shaped surface: complete value type plus function declaration.
struct Celsius {
    double value;
};

double to_fahrenheit(Celsius input) noexcept;

// Source-file-shaped implementation details are not part of the public surface.
namespace detail {
inline constexpr double scale = 9.0 / 5.0;
inline constexpr double offset = 32.0;
}  // namespace detail

double to_fahrenheit(Celsius input) noexcept {
    return input.value * detail::scale + detail::offset;
}

}  // namespace temperature

static_assert(std::is_same_v<decltype(&temperature::to_fahrenheit), double (*)(temperature::Celsius) noexcept>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section03/header_source_separation"};

    LEARN_EXPECT_EQ(checks, temperature::to_fahrenheit({0.0}), 32.0);
    LEARN_EXPECT_EQ(checks, temperature::to_fahrenheit({-40.0}), -40.0);
    LEARN_EXPECT_EQ(checks, temperature::to_fahrenheit({100.0}), 212.0);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/header_source_separation", run>;

}  // namespace
