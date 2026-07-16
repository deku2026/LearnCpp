// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : aggregate_vs_non_aggregate
// Topic id : part2/stage04/section01/aggregate_vs_non_aggregate
// References: [dcl.init.aggr], [class.ctor]

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section01/aggregate_vs_non_aggregate";

struct AggregateRecord {
    int id;
    std::string label;
};

class ValidatedRecord {
public:
    explicit ValidatedRecord(int id) : id_(id < 0 ? 0 : id) {}
    [[nodiscard]] int id() const noexcept { return id_; }

private:
    int id_;
};

static_assert(std::is_aggregate_v<AggregateRecord>);
static_assert(!std::is_aggregate_v<ValidatedRecord>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    AggregateRecord aggregate{.id = 7, .label = "direct member initialization"};
    ValidatedRecord validated{-3};

    LEARN_EXPECT_EQ(checks, aggregate.id, 7);
    LEARN_EXPECT_EQ(checks, aggregate.label, std::string{"direct member initialization"});
    LEARN_EXPECT_EQ(checks, validated.id(), 0);
    LEARN_EXPECT(checks, (std::is_constructible_v<ValidatedRecord, int>));

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section01/aggregate_vs_non_aggregate", run>;

}  // namespace
