// Explicit instantiation chooses where selected template specializations are emitted.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : explicit_template_instantiation
// Topic id : part2/stage06/section01/explicit_template_instantiation
//
// These definitions force int and long specializations in this translation unit.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

template <class T>
constexpr T cube(T value) {
    return value * value * value;
}

template <class T>
class Accumulator {
public:
    void add(T value) { total_ += value; }
    [[nodiscard]] T total() const { return total_; }

private:
    T total_{};
};

template int cube<int>(int);
template class Accumulator<long>;

static_assert(cube(3) == 27);
static_assert(std::is_same_v<decltype(cube(2L)), long>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/explicit_template_instantiation"};
    LEARN_EXPECT_EQ(checks, cube(4), 64);
    Accumulator<long> values;
    values.add(10);
    values.add(5);
    LEARN_EXPECT_EQ(checks, values.total(), 15L);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/explicit_template_instantiation", run>;

}  // namespace
