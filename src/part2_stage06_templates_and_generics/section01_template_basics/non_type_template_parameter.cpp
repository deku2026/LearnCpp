// A non-type template parameter carries a value known during compilation.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : non_type_template_parameter
// Topic id : part2/stage06/section01/non_type_template_parameter
//
// Capacity changes the type itself and can be used directly in array bounds.

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <type_traits>

namespace {

template <class T, std::size_t Capacity>
struct StaticBuffer {
    std::array<T, Capacity> values{};

    [[nodiscard]] static consteval std::size_t capacity() { return Capacity; }
};

template <int Base, unsigned Exponent>
struct IntegerPower {
    static constexpr int value = Base * IntegerPower<Base, Exponent - 1>::value;
};

template <int Base>
struct IntegerPower<Base, 0> {
    static constexpr int value = 1;
};

static_assert(StaticBuffer<int, 4>::capacity() == 4);
static_assert(IntegerPower<3, 4>::value == 81);
static_assert(!std::is_same_v<StaticBuffer<int, 3>, StaticBuffer<int, 4>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/non_type_template_parameter"};
    StaticBuffer<int, 3> buffer{{1, 2, 3}};
    LEARN_EXPECT_EQ(checks, buffer.values[1], 2);
    LEARN_EXPECT_EQ(checks, buffer.capacity(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, (IntegerPower<2, 5>::value), 32);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/non_type_template_parameter", run>;

}  // namespace
