// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section05_pattern_matching_alternatives
// Item     : variant_visit_overload
// Topic id : part2/stage08/section05/variant_visit_overload
//

#include "learn/example_support.hpp"

#include <string>
#include <variant>

namespace {

template <class... Callables>
struct Overloaded : Callables... {
    using Callables::operator()...;
};

template <class... Callables>
Overloaded(Callables...) -> Overloaded<Callables...>;

using Payload = std::variant<int, std::string>;

[[nodiscard]] std::string describe(const Payload& payload) {
    return std::visit(Overloaded{
                          [](const int value) { return std::string{"integer:"} + std::to_string(value); },
                          [](const std::string& value) { return std::string{"text:"} + value; },
                      },
                      payload);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section05/variant_visit_overload"};
    Payload payload = 42;
    LEARN_EXPECT_EQ(checks, describe(payload), std::string{"integer:42"});
    LEARN_EXPECT(checks, std::holds_alternative<int>(payload));

    std::visit(Overloaded{
                   [](int& value) { ++value; },
                   [](std::string& value) { value += '!'; },
               },
               payload);
    LEARN_EXPECT_EQ(checks, std::get<int>(payload), 43);
    LEARN_EXPECT_THROWS(checks, std::bad_variant_access, std::get<std::string>(payload));

    payload = std::string{"C++23"};
    LEARN_EXPECT_EQ(checks, describe(payload), std::string{"text:C++23"});
    LEARN_EXPECT(checks, std::get_if<std::string>(&payload) != nullptr);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section05/variant_visit_overload", run>;

}  // namespace
