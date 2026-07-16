// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_locale
// Topic id : part3/section05/iostream_locale
// References: C++23 [locales], [locale.numpunct], [iostreams.base]

#include "learn/example_support.hpp"

#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section05/iostream_locale";

class EuropeanPunctuation final : public std::numpunct<char> {
private:
    char do_decimal_point() const override { return ','; }
    char do_thousands_sep() const override { return '.'; }
    std::string do_grouping() const override { return "\3"; }
    std::string do_truename() const override { return "yes"; }
    std::string do_falsename() const override { return "no"; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    // Constructing a locale with a new facet transfers ownership to locale.
    const std::locale european{std::locale::classic(), new EuropeanPunctuation};
    std::ostringstream output;
    output.imbue(european);
    output << std::fixed << std::setprecision(2) << 1234567.5 << ' ' << std::boolalpha << true;
    LEARN_EXPECT_EQ(checks, output.str(), std::string{"1.234.567,50 yes"});

    std::istringstream input{"1.234,5 no"};
    input.imbue(european);
    double amount{};
    bool answer = true;
    input >> amount >> std::boolalpha >> answer;
    LEARN_EXPECT(checks, input.good() || input.eof());
    LEARN_EXPECT_EQ(checks, amount, 1234.5);
    LEARN_EXPECT(checks, !answer);

    // Locale is a per-stream policy here. The global locale is deliberately
    // untouched, avoiding process-wide state and machine-installed locale names.
    std::ostringstream classic;
    classic.imbue(std::locale::classic());
    classic << std::fixed << std::setprecision(1) << 1234.5;
    LEARN_EXPECT_EQ(checks, classic.str(), std::string{"1234.5"});
    LEARN_EXPECT_EQ(checks, std::use_facet<std::numpunct<char>>(european).decimal_point(), ',');
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/iostream_locale", run>;

}  // namespace
