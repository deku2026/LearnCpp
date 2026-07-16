// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : virtual_destructor
// Topic id : part2/stage04/section08/virtual_destructor
// References: [class.dtor], [expr.delete]

#include "learn/example_support.hpp"

#include <memory>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section08/virtual_destructor";

struct DestructionLog {
    int base{};
    int derived{};
};

class PolymorphicBase {
public:
    explicit PolymorphicBase(DestructionLog& log) : log_(log) {}
    virtual ~PolymorphicBase() { ++log_.base; }

protected:
    DestructionLog& log_;
};

class PolymorphicDerived : public PolymorphicBase {
public:
    explicit PolymorphicDerived(DestructionLog& log) : PolymorphicBase(log) {}
    ~PolymorphicDerived() override { ++log_.derived; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    DestructionLog log;
    {
        std::unique_ptr<PolymorphicBase> value = std::make_unique<PolymorphicDerived>(log);
    }
    LEARN_EXPECT_EQ(checks, log.derived, 1);
    LEARN_EXPECT_EQ(checks, log.base, 1);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section08/virtual_destructor", run>;

}  // namespace
