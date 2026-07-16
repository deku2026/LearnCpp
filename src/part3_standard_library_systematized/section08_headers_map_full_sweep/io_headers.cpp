// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : io_headers
// Topic id : part3/section08/io_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section08/io_headers";

constexpr std::array<std::string_view, 17> headers{
    "cstdio",     "fstream", "iomanip",   "ios",       "iosfwd",     "iostream", "istream",    "ostream", "print",
    "spanstream", "sstream", "streambuf", "strstream", "syncstream", "format",   "filesystem", "locale",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::ostringstream output;
    output << std::hex << std::showbase << 255;
    LEARN_EXPECT_EQ(checks, output.str(), std::string{"0xff"});
    std::istringstream input{"12 bad"};
    int value{};
    input >> value;
    LEARN_EXPECT_EQ(checks, value, 12);
    input >> value;
    LEARN_EXPECT(checks, input.fail());
    LEARN_EXPECT_EQ(checks, headers.size(), 17U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/io_headers", run>;

}  // namespace
