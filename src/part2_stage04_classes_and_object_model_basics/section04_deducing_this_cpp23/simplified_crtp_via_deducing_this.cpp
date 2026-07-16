// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : simplified_crtp_via_deducing_this
// Topic id : part2/stage04/section04/simplified_crtp_via_deducing_this
// References: [dcl.fct], P0847R7

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section04/simplified_crtp_via_deducing_this";

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
struct Doubleable {
    void double_value(this auto& self) noexcept { self.value *= 2; }
};

struct Number : Doubleable {
    int value{};
};
#else
template <class Derived>
struct Doubleable {
    void double_value() noexcept { static_cast<Derived&>(*this).value *= 2; }
};

struct Number : Doubleable<Number> {
    int value{};
};
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Number number;
    number.value = 6;
    number.double_value();
    LEARN_EXPECT_EQ(checks, number.value, 12);

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage04/section04/simplified_crtp_via_deducing_this", run>;

}  // namespace
