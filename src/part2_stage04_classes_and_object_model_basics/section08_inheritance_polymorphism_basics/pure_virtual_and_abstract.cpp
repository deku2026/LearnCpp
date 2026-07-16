// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : pure_virtual_and_abstract
// Topic id : part2/stage04/section08/pure_virtual_and_abstract
// References: [class.abstract]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section08/pure_virtual_and_abstract";

struct Formatter {
    virtual ~Formatter() = default;
    [[nodiscard]] virtual int width(int value) const = 0;
};

struct DecimalFormatter : Formatter {
    [[nodiscard]] int width(int value) const override {
        int digits = value == 0 ? 1 : 0;
        for (; value != 0; value /= 10) {
            ++digits;
        }
        return digits;
    }
};

static_assert(std::is_abstract_v<Formatter>);
static_assert(!std::is_abstract_v<DecimalFormatter>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const DecimalFormatter formatter;
    const Formatter& interface = formatter;
    LEARN_EXPECT_EQ(checks, interface.width(0), 1);
    LEARN_EXPECT_EQ(checks, interface.width(1234), 4);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section08/pure_virtual_and_abstract", run>;

}  // namespace
