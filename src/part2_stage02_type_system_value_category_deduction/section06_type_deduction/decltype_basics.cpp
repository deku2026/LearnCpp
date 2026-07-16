// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : decltype_basics
// Topic id : part2/stage02/section06/decltype_basics
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct Reading {
    double value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    const int number = 4;
    Reading reading{2.5};

    static_assert(std::is_same_v<decltype(number), const int>);
    static_assert(std::is_same_v<decltype((number)), const int&>);
    static_assert(std::is_same_v<decltype(reading.value), double>);
    static_assert(std::is_same_v<decltype((reading.value)), double&>);

    decltype(reading.value) copied = reading.value;
    decltype((reading.value)) aliased = reading.value;
    aliased = 3.5;

    learn::ExampleChecks checks{"part2/stage02/section06/decltype_basics"};
    LEARN_EXPECT_EQ(checks, copied, 2.5);
    LEARN_EXPECT_EQ(checks, reading.value, 3.5);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/decltype_basics", run>;

}  // namespace
