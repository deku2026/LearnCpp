// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : virtual_functions
// Topic id : part2/stage04/section08/virtual_functions
// References: [class.virtual], [expr.call]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section08/virtual_functions";

class Animal {
public:
    virtual ~Animal() = default;
    [[nodiscard]] virtual std::string_view sound() const noexcept { return "?"; }
    [[nodiscard]] std::string category() const { return "animal"; }
};

class Dog : public Animal {
public:
    [[nodiscard]] std::string_view sound() const noexcept override { return "woof"; }
    [[nodiscard]] std::string category() const { return "dog"; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const Dog dog;
    const Animal& animal = dog;
    LEARN_EXPECT_EQ(checks, animal.sound(), std::string_view{"woof"});  // virtual dispatch
    LEARN_EXPECT_EQ(checks, animal.category(), std::string{"animal"});  // non-virtual static binding
    LEARN_EXPECT_EQ(checks, dog.category(), std::string{"dog"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section08/virtual_functions", run>;

}  // namespace
