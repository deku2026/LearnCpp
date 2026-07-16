// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_cpp20
// Topic id : part3/section01/std_format_cpp20
// Reference: C++ working draft [format]; WG21 P0645R10.

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

#include <iterator>
#include <string>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/std_format_cpp20";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_FORMAT_HEADER && defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    LEARN_EXPECT_EQ(checks, std::format("{} + {} = {}", 1, 2, 3), std::string{"1 + 2 = 3"});
    LEARN_EXPECT_EQ(checks, std::format("{:>8}", "hi"), std::string{"      hi"});
    LEARN_EXPECT_EQ(checks, std::format("{:08.2f}", 3.5), std::string{"00003.50"});
    LEARN_EXPECT_EQ(checks, std::format("{:#x}", 255), std::string{"0xff"});
    LEARN_EXPECT_EQ(checks, std::format("{{{}}}", 42), std::string{"{42}"});

    std::string destination{"prefix:"};
    std::format_to(std::back_inserter(destination), "{}:{}", "item", 7);
    LEARN_EXPECT_EQ(checks, destination, std::string{"prefix:item:7"});
    LEARN_EXPECT_EQ(checks, std::formatted_size("{}:{}", "item", 7), std::size_t{6});
    // Literal format strings are parsed and type-checked at compile time; formatting is still O(output size).
#else
    const std::string modeled = std::to_string(1) + " + " + std::to_string(2) + " = " + std::to_string(3);
    LEARN_EXPECT_EQ(checks, modeled, std::string{"1 + 2 = 3"});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_format >= 201907L and <format>");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_format_cpp20", run>;

}  // namespace
