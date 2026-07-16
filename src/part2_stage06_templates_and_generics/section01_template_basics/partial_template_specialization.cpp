// Partial specialization selects a class-template definition for a type pattern.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : partial_template_specialization
// Topic id : part2/stage06/section01/partial_template_specialization
//
// Pointer and built-in-array families are matched without enumerating exact types.

#include "learn/example_support.hpp"

#include <cstddef>
#include <string_view>

namespace {

template <class T>
struct TypeShape {
    static constexpr std::string_view name = "scalar";
    static constexpr std::size_t extent = 0;
};

template <class T>
struct TypeShape<T*> {
    static constexpr std::string_view name = "pointer";
    static constexpr std::size_t extent = 0;
};

template <class T, std::size_t Size>
struct TypeShape<T[Size]> {
    static constexpr std::string_view name = "array";
    static constexpr std::size_t extent = Size;
};

static_assert(TypeShape<int*>::name == "pointer");
static_assert(TypeShape<double[5]>::extent == 5);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/partial_template_specialization"};
    LEARN_EXPECT_EQ(checks, TypeShape<int>::name, std::string_view{"scalar"});
    LEARN_EXPECT_EQ(checks, TypeShape<const int*>::name, std::string_view{"pointer"});
    LEARN_EXPECT_EQ(checks, TypeShape<char[4]>::name, std::string_view{"array"});
    LEARN_EXPECT_EQ(checks, TypeShape<char[4]>::extent, std::size_t{4});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/partial_template_specialization", run>;

}  // namespace
