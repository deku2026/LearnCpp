// CRTP is static dispatch; virtual functions preserve runtime heterogeneity.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_versus_virtual
// Topic id : part2/stage06/section06/crtp_versus_virtual
//
// Both compute the same values, but only the virtual interface shares one runtime base type.

#include "learn/example_support.hpp"

#include <array>
#include <type_traits>

namespace {

template <class Derived>
struct StaticValue {
    [[nodiscard]] int value() const { return static_cast<const Derived&>(*this).value_impl(); }
};

struct StaticTen : StaticValue<StaticTen> {
    [[nodiscard]] int value_impl() const { return 10; }
};

struct StaticTwenty : StaticValue<StaticTwenty> {
    [[nodiscard]] int value_impl() const { return 20; }
};

struct DynamicValue {
    virtual ~DynamicValue() = default;
    [[nodiscard]] virtual int value() const = 0;
};

struct DynamicTen : DynamicValue {
    [[nodiscard]] int value() const override { return 10; }
};

struct DynamicTwenty : DynamicValue {
    [[nodiscard]] int value() const override { return 20; }
};

static_assert(!std::is_polymorphic_v<StaticTen>);
static_assert(std::is_polymorphic_v<DynamicValue>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section06/crtp_versus_virtual"};
    const StaticTen static_ten;
    const StaticTwenty static_twenty;
    LEARN_EXPECT_EQ(checks, static_ten.value() + static_twenty.value(), 30);

    const DynamicTen dynamic_ten;
    const DynamicTwenty dynamic_twenty;
    const std::array<const DynamicValue*, 2> heterogeneous{&dynamic_ten, &dynamic_twenty};
    LEARN_EXPECT_EQ(checks, heterogeneous[0]->value() + heterogeneous[1]->value(), 30);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_versus_virtual", run>;

}  // namespace
