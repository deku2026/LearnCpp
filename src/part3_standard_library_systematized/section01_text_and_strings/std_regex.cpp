// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_regex
// Topic id : part3/section01/std_regex
// Reference: C++ working draft [re]; default grammar is ECMAScript.

#include "learn/example_support.hpp"

#if __has_include(<regex>)
#include <regex>
#define LEARN_HAS_REGEX_HEADER 1
#else
#define LEARN_HAS_REGEX_HEADER 0
#endif

#include <string>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/std_regex";

#if LEARN_HAS_REGEX_HEADER
    learn::ExampleChecks checks{topic};
    const std::regex email{R"(([^@\s]+)@([^@\s]+))"};
    std::smatch match;
    const std::string address{"ada@example.com"};
    LEARN_EXPECT(checks, std::regex_match(address, match, email));
    LEARN_EXPECT_EQ(checks, match.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, match[1].str(), std::string{"ada"});
    LEARN_EXPECT_EQ(checks, match[2].str(), std::string{"example.com"});

    const std::string log{"id=17; next=42"};
    const std::regex number{R"(\d+)"};
    std::vector<int> values;
    for (std::sregex_iterator iterator{log.begin(), log.end(), number}, end; iterator != end; ++iterator) {
        values.push_back(std::stoi(iterator->str()));
    }
    LEARN_EXPECT_EQ(checks, values, (std::vector{17, 42}));
    LEARN_EXPECT_EQ(checks, std::regex_replace(log, number, "#"), std::string{"id=#; next=#"});
    LEARN_EXPECT_THROWS(checks, std::regex_error, std::regex{"(unterminated"});
    // Pattern compilation can be expensive and matching complexity is implementation/pattern dependent.
    // Reuse compiled regex objects and prefer linear-time engines for hostile or latency-sensitive input.
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic, "standard <regex> header");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_regex", run>;

}  // namespace
