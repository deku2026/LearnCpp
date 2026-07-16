// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : dynamic_cast
// Topic id : part6/a/section03/dynamic_cast
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <stdexcept>
#include <string_view>
#include <typeinfo>

namespace {

constexpr std::string_view kTopic = "part6/a/section03/dynamic_cast";

struct Animal {
    virtual ~Animal() = default;
};
struct Cat : Animal {
    int lives{9};
};
struct Dog : Animal {
    int bones{2};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Cat cat;
    Animal* animal = &cat;
    LEARN_EXPECT(checks, dynamic_cast<Cat*>(animal) == &cat);
    LEARN_EXPECT(checks, dynamic_cast<Dog*>(animal) == nullptr);
    LEARN_EXPECT_EQ(checks, dynamic_cast<Cat&>(*animal).lives, 9);
    LEARN_EXPECT_THROWS(checks, std::bad_cast, dynamic_cast<Dog&>(*animal));
    LEARN_EXPECT(checks, dynamic_cast<void*>(animal) == &cat);  // Most-derived object address.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section03/dynamic_cast", run>;

}  // namespace
