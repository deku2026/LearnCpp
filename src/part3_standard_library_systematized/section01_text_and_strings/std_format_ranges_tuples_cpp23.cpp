// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_ranges_tuples_cpp23
// Topic id : part3/section01/std_format_ranges_tuples_cpp23
// Reference: WG21 P2286R8 (__cpp_lib_format_ranges).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<format>)
#include <format>
#define LEARN_HAS_FORMAT_HEADER 1
#else
#define LEARN_HAS_FORMAT_HEADER 0
#endif

#include <string>
#include <tuple>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/std_format_ranges_tuples_cpp23";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_FORMAT_HEADER && defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && \
    defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L &&              \
    (!defined(LEARNCPP_HAS_FORMAT_RANGES) || LEARNCPP_HAS_FORMAT_RANGES)
    const std::vector values{1, 2, 3};
    const std::tuple record{"Ada", 23};
    LEARN_EXPECT_EQ(checks, std::format("{}", values), std::string{"[1, 2, 3]"});
    LEARN_EXPECT_EQ(checks, std::format("{}", record), std::string{"(\"Ada\", 23)"});
    LEARN_EXPECT_EQ(checks, std::format("{::02}", values), std::string{"[01, 02, 03]"});

    const std::vector<std::vector<int>> matrix{{1, 2}, {3, 4}};
    LEARN_EXPECT_EQ(checks, std::format("{}", matrix), std::string{"[[1, 2], [3, 4]]"});
    LEARN_EXPECT_EQ(checks, std::format("{}", std::vector<int>{}), std::string{"[]"});
    // Range formatting traverses the range and is O(N + output size); it does not materialize a second range.
#else
    const std::vector values{1, 2, 3};
    std::string modeled{"["};
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) {
            modeled += ", ";
        }
        modeled += std::to_string(values[index]);
    }
    modeled += ']';
    LEARN_EXPECT_EQ(checks, modeled, std::string{"[1, 2, 3]"});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "compile-and-link usable __cpp_lib_format_ranges >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_format_ranges_tuples_cpp23", run>;

}  // namespace
