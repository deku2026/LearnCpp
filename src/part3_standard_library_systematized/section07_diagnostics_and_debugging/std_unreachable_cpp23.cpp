// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : std_unreachable_cpp23
// Topic id : part3/section07/std_unreachable_cpp23
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <string_view>
#include <utility>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section07/std_unreachable_cpp23";

enum class Token { word, number };

constexpr std::string_view name(Token token) {
    switch (token) {
        case Token::word:
            return "word";
        case Token::number:
            return "number";
    }
#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
    std::unreachable();
#else
    return "invalid";
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    static_assert(name(Token::word) == "word");
    LEARN_EXPECT_EQ(checks, name(Token::number), std::string_view{"number"});
    // Reaching std::unreachable is undefined; input validation belongs before this closed-domain helper.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section07/std_unreachable_cpp23", run>;

}  // namespace
