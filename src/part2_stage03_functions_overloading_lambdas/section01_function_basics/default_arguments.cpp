// Default arguments are substituted and evaluated at each call site.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : default_arguments
// Topic id : part2/stage03/section01/default_arguments
//
// A default belongs on a visible declaration.  It is not part of the
// function type and is not repeated on the definition.

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <type_traits>

namespace {

std::string greeting(std::string_view name, std::string_view salutation = "Hello", char punctuation = '!');

std::string greeting(std::string_view name, std::string_view salutation, char punctuation) {
    std::string result{salutation};
    result += ", ";
    result += name;
    result += punctuation;
    return result;
}

int& ticket_seed() {
    static int seed = 40;
    return seed;
}

int issue_ticket() {
    return ++ticket_seed();
}

int ticket(int number = issue_ticket()) {
    return number;
}

using GreetingFunction = std::string (*)(std::string_view, std::string_view, char);
static_assert(std::is_same_v<decltype(&greeting), GreetingFunction>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage03/section01/default_arguments"};
    LEARN_EXPECT_EQ(checks, greeting("Ada"), std::string{"Hello, Ada!"});
    LEARN_EXPECT_EQ(checks, greeting("Bjarne", "Welcome"), std::string{"Welcome, Bjarne!"});
    LEARN_EXPECT_EQ(checks, greeting("C++", "Hi", '?'), std::string{"Hi, C++?"});

    // A function pointer carries only the function type, so all arguments are explicit here.
    GreetingFunction callable = &greeting;
    LEARN_EXPECT_EQ(checks, callable("caller", "Direct", '.'), std::string{"Direct, caller."});

    ticket_seed() = 40;
    LEARN_EXPECT_EQ(checks, ticket(), 41);
    LEARN_EXPECT_EQ(checks, ticket(), 42);
    LEARN_EXPECT_EQ(checks, ticket(99), 99);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/default_arguments", run>;

}  // namespace
