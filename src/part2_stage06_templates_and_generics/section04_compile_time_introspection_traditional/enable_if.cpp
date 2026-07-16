// enable_if turns a boolean trait into a traditional SFINAE switch.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : enable_if
// Topic id : part2/stage06/section04/enable_if
//
// Each overload exists only for its numeric category; unsupported types are queried safely.

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>

namespace {

enum class NumericKind { integral, floating_point };

template <class T>
std::enable_if_t<std::is_integral_v<T>, NumericKind> numeric_kind(T) {
    return NumericKind::integral;
}

template <class T>
std::enable_if_t<std::is_floating_point_v<T>, NumericKind> numeric_kind(T) {
    return NumericKind::floating_point;
}

template <class T>
concept HasNumericKind = requires(T value) { numeric_kind(value); };

static_assert(HasNumericKind<int>);
static_assert(HasNumericKind<double>);
static_assert(!HasNumericKind<std::string>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section04/enable_if"};
    LEARN_EXPECT(checks, numeric_kind(7) == NumericKind::integral);
    LEARN_EXPECT(checks, numeric_kind(2.5) == NumericKind::floating_point);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/enable_if", run>;

}  // namespace
