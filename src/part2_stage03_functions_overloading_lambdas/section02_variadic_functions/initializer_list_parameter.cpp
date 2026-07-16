// initializer_list accepts a variable number of values of one element type.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section02_variadic_functions
// Item     : initializer_list_parameter
// Topic id : part2/stage03/section02/initializer_list_parameter
//
// Its elements are const, its size is known, and an empty list is a normal
// input that the callee should handle explicitly.

#include "learn/example_support.hpp"

#include <cstddef>
#include <initializer_list>
#include <optional>
#include <type_traits>

namespace {

struct Summary {
    std::size_t count{};
    int total{};
    std::optional<int> minimum;
};

Summary summarize(std::initializer_list<int> values) {
    Summary result{values.size(), 0, std::nullopt};
    for (const int value : values) {
        result.total += value;
        if (!result.minimum || value < *result.minimum) {
            result.minimum = value;
        }
    }
    return result;
}

static_assert(std::is_same_v<std::initializer_list<int>::iterator, const int*>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage03/section02/initializer_list_parameter"};

    const Summary values = summarize({5, 2, 8, 3});
    LEARN_EXPECT_EQ(checks, values.count, std::size_t{4});
    LEARN_EXPECT_EQ(checks, values.total, 18);
    LEARN_EXPECT(checks, values.minimum.has_value());
    LEARN_EXPECT_EQ(checks, *values.minimum, 2);

    const Summary empty = summarize({});
    LEARN_EXPECT_EQ(checks, empty.count, std::size_t{0});
    LEARN_EXPECT_EQ(checks, empty.total, 0);
    LEARN_EXPECT(checks, !empty.minimum.has_value());
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section02/initializer_list_parameter", run>;

}  // namespace
