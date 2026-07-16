// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : type_info
// Topic id : part6/a/section03/type_info
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <set>
#include <string_view>
#include <typeindex>
#include <typeinfo>

namespace {

constexpr std::string_view kTopic = "part6/a/section03/type_info";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::type_info& integer = typeid(int);
    const std::type_info& real = typeid(double);
    LEARN_EXPECT(checks, integer == typeid(int));
    LEARN_EXPECT(checks, integer != real);
    LEARN_EXPECT(checks, std::string_view{integer.name()}.size() > 0U);  // Spelling is implementation-defined.
    const std::set<std::type_index> registry{typeid(int), typeid(double), typeid(int)};
    LEARN_EXPECT_EQ(checks, registry.size(), 2U);
    LEARN_EXPECT(checks, registry.contains(std::type_index{typeid(double)}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section03/type_info", run>;

}  // namespace
