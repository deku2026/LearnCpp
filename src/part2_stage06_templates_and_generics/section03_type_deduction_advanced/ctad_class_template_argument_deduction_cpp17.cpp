// C++17 CTAD deduces a class template argument list from construction syntax.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : ctad_class_template_argument_deduction_cpp17
// Topic id : part2/stage06/section03/ctad_class_template_argument_deduction_cpp17
//
// Constructor-generated implicit guides make Box{42} a Box<int>.

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

template <class T>
struct Box {
    T value;

    explicit Box(T initial) : value(std::move(initial)) {}
};

static_assert(std::is_same_v<decltype(Box{42}), Box<int>>);
static_assert(std::is_same_v<decltype(Box{std::string{"text"}}), Box<std::string>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section03/ctad_class_template_argument_deduction_cpp17"};
    Box integer{42};
    Box text{std::string{"deduced"}};
    std::pair pair{1, 2.5};
    static_assert(std::is_same_v<decltype(pair), std::pair<int, double>>);
    LEARN_EXPECT_EQ(checks, integer.value, 42);
    LEARN_EXPECT_EQ(checks, text.value, std::string{"deduced"});
    LEARN_EXPECT_EQ(checks, pair.second, 2.5);
    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage06/section03/ctad_class_template_argument_deduction_cpp17", run>;

}  // namespace
