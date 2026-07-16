// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : aggregate_initialization_rules
// Topic id : part2/stage02/section09/aggregate_initialization_rules
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct Base {
    int base;
};

struct Aggregate : Base {
    int value = 7;
    int omitted = 0;
};

class NonAggregate {
public:
    explicit NonAggregate(int input) : value_(input) {}
    int value() const { return value_; }

private:
    int value_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_aggregate_v<Aggregate>);
    static_assert(!std::is_aggregate_v<NonAggregate>);

    Aggregate aggregate{{3}, 8};  // omitted is initialized from an empty list.
    NonAggregate object{9};

    learn::ExampleChecks checks{"part2/stage02/section09/aggregate_initialization_rules"};
    LEARN_EXPECT_EQ(checks, aggregate.base, 3);
    LEARN_EXPECT_EQ(checks, aggregate.value, 8);
    LEARN_EXPECT_EQ(checks, aggregate.omitted, 0);
    LEARN_EXPECT_EQ(checks, object.value(), 9);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section09/aggregate_initialization_rules", run>;

}  // namespace
