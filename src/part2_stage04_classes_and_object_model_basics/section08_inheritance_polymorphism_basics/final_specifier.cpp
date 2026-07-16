// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : final_specifier
// Topic id : part2/stage04/section08/final_specifier
// References: [class], [class.virtual]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section08/final_specifier";

struct BaseOperation {
    virtual ~BaseOperation() = default;
    virtual int apply(int value) const { return value; }
};

struct Doubler final : BaseOperation {
    int apply(int value) const override { return value * 2; }
};

static_assert(std::is_final_v<Doubler>);
static_assert(std::is_base_of_v<BaseOperation, Doubler>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const Doubler doubler;
    const BaseOperation& operation = doubler;
    LEARN_EXPECT_EQ(checks, operation.apply(6), 12);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section08/final_specifier", run>;

}  // namespace
