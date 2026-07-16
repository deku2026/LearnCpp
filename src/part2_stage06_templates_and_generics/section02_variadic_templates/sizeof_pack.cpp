// sizeof... reports a pack's arity as a compile-time std::size_t value.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : sizeof_pack
// Topic id : part2/stage06/section02/sizeof_pack
//
// It works for both template type packs and function parameter packs, including empty packs.

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace {

template <class... Types>
inline constexpr std::size_t type_count_v = sizeof...(Types);

template <class... Arguments>
constexpr std::size_t argument_count(Arguments&&...) noexcept {
    return sizeof...(Arguments);
}

template <class... Types>
using HasExactlyTwo = std::bool_constant<sizeof...(Types) == 2>;

static_assert(type_count_v<> == 0);
static_assert(type_count_v<int, double, char> == 3);
static_assert(HasExactlyTwo<int, long>::value);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section02/sizeof_pack"};
    LEARN_EXPECT_EQ(checks, argument_count(), std::size_t{0});
    LEARN_EXPECT_EQ(checks, argument_count(1, 2.0, 'x'), std::size_t{3});
    int value = 1;
    LEARN_EXPECT_EQ(checks, argument_count(value, std::move(value)), std::size_t{2});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/sizeof_pack", run>;

}  // namespace
