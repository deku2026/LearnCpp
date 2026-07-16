// Function parameter ownership and optionality examples.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : pass_by_value_reference_const_ref_pointer
// Topic id : part2/stage03/section01/pass_by_value_reference_const_ref_pointer
//
// Value parameters own an independent object, references require an object,
// and pointers can explicitly represent "no object".

#include "learn/example_support.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

namespace {

std::string by_value(std::string text) {
    text += '!';
    return text;
}

void by_reference(std::string& text) {
    text += " changed";
}

std::size_t by_const_reference(const std::string& text) {
    return text.size();
}

bool append_if_present(std::string* text, std::string_view suffix) {
    if (text == nullptr) {
        return false;
    }
    text->append(suffix);
    return true;
}

static_assert(std::is_invocable_r_v<std::size_t, decltype(&by_const_reference), const std::string&>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage03/section01/pass_by_value_reference_const_ref_pointer"};

    std::string original{"value"};
    const std::string changed_copy = by_value(original);
    LEARN_EXPECT_EQ(checks, original, std::string{"value"});
    LEARN_EXPECT_EQ(checks, changed_copy, std::string{"value!"});

    by_reference(original);
    LEARN_EXPECT_EQ(checks, original, std::string{"value changed"});
    LEARN_EXPECT_EQ(checks, by_const_reference(original), std::size_t{13});
    LEARN_EXPECT_EQ(checks, by_const_reference(std::string{"temporary"}), std::size_t{9});

    LEARN_EXPECT(checks, append_if_present(&original, " safely"));
    LEARN_EXPECT(checks, !append_if_present(nullptr, " ignored"));
    LEARN_EXPECT_EQ(checks, original, std::string{"value changed safely"});
    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage03/section01/pass_by_value_reference_const_ref_pointer", run>;

}  // namespace
