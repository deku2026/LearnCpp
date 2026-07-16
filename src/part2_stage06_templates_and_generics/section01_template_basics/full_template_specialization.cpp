// A full specialization replaces a primary class template for one exact type.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : full_template_specialization
// Topic id : part2/stage06/section01/full_template_specialization
//
// Other types continue to use the primary definition.

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>

namespace {

template <class T>
struct TypeCategory {
    static constexpr std::string_view name = "general";
    static constexpr int code = 0;
};

template <>
struct TypeCategory<bool> {
    static constexpr std::string_view name = "boolean";
    static constexpr int code = 1;
};

static_assert(TypeCategory<int>::code == 0);
static_assert(TypeCategory<bool>::code == 1);
static_assert(!std::is_same_v<TypeCategory<int>, TypeCategory<bool>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/full_template_specialization"};
    LEARN_EXPECT_EQ(checks, TypeCategory<double>::name, std::string_view{"general"});
    LEARN_EXPECT_EQ(checks, TypeCategory<bool>::name, std::string_view{"boolean"});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/full_template_specialization", run>;

}  // namespace
