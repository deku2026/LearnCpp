// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_state_flags
// Topic id : part3/section05/iostream_state_flags
// References: C++23 [ios.base], [iostate.flags], [istream.formatted]

#include "learn/example_support.hpp"

#include <ios>
#include <sstream>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section05/iostream_state_flags";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::istringstream input{"42 nope 7"};
    int value{};
    input >> value;
    LEARN_EXPECT_EQ(checks, value, 42);
    LEARN_EXPECT(checks, input.good());

    input >> value;
    LEARN_EXPECT(checks, input.fail());
    LEARN_EXPECT(checks, !input.bad());
    LEARN_EXPECT(checks, !input.eof());
    const auto failed_state = input.rdstate();
    LEARN_EXPECT(checks, (failed_state & std::ios_base::failbit) != std::ios_base::goodbit);

    // Formatted extraction is inert until clear() removes failbit.
    value = -1;
    input >> value;
    LEARN_EXPECT_EQ(checks, value, -1);
    input.clear();
    std::string rejected_token;
    input >> rejected_token >> value;
    LEARN_EXPECT_EQ(checks, rejected_token, std::string{"nope"});
    LEARN_EXPECT_EQ(checks, value, 7);

    // eofbit is normally observed only after an operation attempts to read
    // through the end. Depending on extraction, failbit can accompany it.
    input >> value;
    LEARN_EXPECT(checks, input.eof());
    LEARN_EXPECT(checks, input.fail());
    input.clear();
    LEARN_EXPECT(checks, input.good());

    std::istringstream throwing{"not-an-int"};
    throwing.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    LEARN_EXPECT_THROWS(checks, std::ios_base::failure, throwing >> value);
    LEARN_EXPECT(checks, throwing.fail());
    // Disable the exception mask before clearing or reusing a throwing stream.
    throwing.exceptions(std::ios_base::goodbit);
    throwing.clear();
    LEARN_EXPECT(checks, throwing.good());

    std::ostringstream output;
    output.setstate(std::ios_base::badbit);
    output << "discarded";
    LEARN_EXPECT(checks, output.bad());
    LEARN_EXPECT(checks, output.str().empty());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/iostream_state_flags", run>;

}  // namespace
