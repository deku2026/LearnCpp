// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : sstream_overview
// Topic id : part3/section05/sstream_overview
// References: C++23 [stringstream.syn], [istringstream], [ostringstream]

#include "learn/example_support.hpp"

#include <iomanip>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section05/sstream_overview";

std::optional<int> parse_integer_exactly(std::string_view text) {
    std::istringstream input{std::string{text}};
    int value{};
    if (!(input >> value)) {
        return std::nullopt;
    }
    input >> std::ws;
    if (!input.eof()) {
        return std::nullopt;
    }
    return value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, parse_integer_exactly(" 42 ").value_or(-1), 42);
    LEARN_EXPECT(checks, !parse_integer_exactly("42ms"));
    LEARN_EXPECT(checks, !parse_integer_exactly("999999999999999999999"));

    std::istringstream csv{R"("Ada, A.",36,"compiler pioneer")"};
    std::string name;
    std::string note;
    int age{};
    char comma1{};
    char comma2{};
    csv >> std::quoted(name) >> comma1 >> age >> comma2 >> std::quoted(note);
    LEARN_EXPECT(checks, csv.good() || csv.eof());
    LEARN_EXPECT_EQ(checks, name, std::string{"Ada, A."});
    LEARN_EXPECT_EQ(checks, age, 36);
    LEARN_EXPECT_EQ(checks, note, std::string{"compiler pioneer"});
    LEARN_EXPECT(checks, comma1 == ',' && comma2 == ',');

    std::ostringstream builder;
    builder << std::left << std::setw(8) << "items" << std::right << std::setw(4) << 3;
    LEARN_EXPECT_EQ(checks, builder.str(), std::string{"items      3"});

    // Replacing a string buffer does not clear prior state bits. Reset both
    // when reusing a stream after a failed parse.
    std::istringstream reusable{"bad"};
    int value{};
    reusable >> value;
    LEARN_EXPECT(checks, reusable.fail());
    reusable.str("17 19");
    LEARN_EXPECT(checks, reusable.fail());
    reusable.clear();
    std::vector<int> values;
    while (reusable >> value) {
        values.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, values, (std::vector<int>{17, 19}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/sstream_overview", run>;

}  // namespace
