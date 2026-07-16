// Template metaprogramming can be viewed as a compile-time function from types to types.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : compile_time_type_calculation
// Topic id : part2/stage06/section07/compile_time_type_calculation
//
// Partial specialization recursively strips pointers; conditional_t chooses by a type property.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

template <class T>
struct RemoveAllPointers {
    using type = T;
};

template <class T>
struct RemoveAllPointers<T*> {
    using type = typename RemoveAllPointers<T>::type;
};

template <class T>
using RemoveAllPointersT = typename RemoveAllPointers<T>::type;

template <class Left, class Right>
using WiderType = std::conditional_t<(sizeof(Left) >= sizeof(Right)), Left, Right>;

static_assert(std::is_same_v<RemoveAllPointersT<const int***>, const int>);
static_assert(sizeof(WiderType<char, long long>) == sizeof(long long));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section07/compile_time_type_calculation"};
    LEARN_EXPECT(checks, (std::is_same_v<RemoveAllPointersT<double**>, double>));
    LEARN_EXPECT_EQ(checks, sizeof(WiderType<short, double>), sizeof(double));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/compile_time_type_calculation", run>;

}  // namespace
