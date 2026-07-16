// type_traits query and transform types without constructing any values.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : type_traits_is_same_and_friends
// Topic id : part2/stage06/section04/type_traits_is_same_and_friends
//
// _v exposes a predicate value; _t exposes a transformed type.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

template <class T>
using Normalized = std::remove_cv_t<std::remove_reference_t<T>>;

template <class T>
inline constexpr bool scalar_number_v = std::is_integral_v<Normalized<T>> || std::is_floating_point_v<Normalized<T>>;

static_assert(std::is_same_v<Normalized<const int&>, int>);
static_assert(std::is_pointer_v<int*>);
static_assert(std::is_same_v<std::conditional_t<true, long, double>, long>);
static_assert(scalar_number_v<const double&>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section04/type_traits_is_same_and_friends"};
    LEARN_EXPECT(checks, scalar_number_v<int>);
    LEARN_EXPECT(checks, scalar_number_v<const double&>);
    LEARN_EXPECT(checks, !scalar_number_v<void*>);
    LEARN_EXPECT(checks, (std::is_same_v<Normalized<volatile long&&>, long>));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/type_traits_is_same_and_friends", run>;

}  // namespace
