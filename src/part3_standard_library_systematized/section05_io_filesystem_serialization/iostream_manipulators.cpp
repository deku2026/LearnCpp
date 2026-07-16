// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_manipulators
// Topic id : part3/section05/iostream_manipulators
// References: C++23 [std.manip], [facet.num.put.virtuals]

#include "learn/example_support.hpp"

#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section05/iostream_manipulators";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::ostringstream output;
    output << std::boolalpha << true << ' ';
    output << std::hex << std::showbase << 42 << ' ';
    output << std::dec << std::noshowbase << std::fixed << std::setprecision(2) << 3.5;
    LEARN_EXPECT_EQ(checks, output.str(), std::string{"true 0x2a 3.50"});

    // basefield, floatfield, boolalpha, precision and fill are sticky.
    output.str({});
    output.clear();
    output << 2.0 << ' ' << false;
    LEARN_EXPECT_EQ(checks, output.str(), std::string{"2.00 false"});

    // setw applies to one insertion only; setfill remains in effect.
    output.str({});
    output << std::setfill('0') << std::setw(4) << 7 << '|' << 8;
    LEARN_EXPECT_EQ(checks, output.str(), std::string{"0007|8"});

    // std::quoted escapes quotes and backslashes and reverses that encoding.
    const std::string original = R"(C:\docs\"draft".txt)";
    output.str({});
    output << std::quoted(original);
    std::istringstream input{output.str()};
    std::string decoded;
    input >> std::quoted(decoded);
    LEARN_EXPECT_EQ(checks, decoded, original);

    // copyfmt is useful when a temporary formatting policy must be restored.
    std::ostringstream saved_format;
    saved_format.copyfmt(output);
    output << std::scientific << std::setprecision(1);
    output.copyfmt(saved_format);
    LEARN_EXPECT_EQ(checks, output.precision(), saved_format.precision());
    LEARN_EXPECT_EQ(checks, output.fill(), saved_format.fill());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/iostream_manipulators", run>;

}  // namespace
