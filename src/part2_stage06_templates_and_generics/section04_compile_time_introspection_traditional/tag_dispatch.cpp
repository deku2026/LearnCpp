// Tag dispatch converts a trait result into an overload-selection argument.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : tag_dispatch
// Topic id : part2/stage06/section04/tag_dispatch
//
// true_type and false_type select different valid implementations before Concepts.

#include "learn/example_support.hpp"

#include <cstddef>
#include <string>
#include <type_traits>

namespace {

struct DispatchResult {
    std::size_t value{};
    bool integral_path{};
};

template <class T>
DispatchResult inspect(const T& value, std::true_type) {
    const auto magnitude = value < 0 ? -value : value;
    return {static_cast<std::size_t>(magnitude), true};
}

template <class T>
DispatchResult inspect(const T& value, std::false_type) {
    return {value.size(), false};
}

template <class T>
DispatchResult inspect(const T& value) {
    return inspect(value, std::is_integral<T>{});
}

static_assert(std::is_same_v<decltype(std::is_integral<int>{}), std::is_integral<int>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section04/tag_dispatch"};
    const auto integer = inspect(-12);
    const auto text = inspect(std::string{"tags"});
    LEARN_EXPECT_EQ(checks, integer.value, std::size_t{12});
    LEARN_EXPECT(checks, integer.integral_path);
    LEARN_EXPECT_EQ(checks, text.value, std::size_t{4});
    LEARN_EXPECT(checks, !text.integral_path);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/tag_dispatch", run>;

}  // namespace
