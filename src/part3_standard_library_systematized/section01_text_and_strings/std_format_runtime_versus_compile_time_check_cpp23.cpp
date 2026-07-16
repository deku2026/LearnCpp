// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_runtime_versus_compile_time_check_cpp23
// Topic id : part3/section01/std_format_runtime_versus_compile_time_check_cpp23
// Reference: C++ working draft [format.string], [format.functions].

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
#include <string_view>
#include <utility>

namespace {

#if LEARN_HAS_FORMAT_HEADER && defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
template <class... Arguments>
std::string checked_format(std::format_string<Arguments...> pattern, Arguments&&... arguments) {
    return std::format(pattern, std::forward<Arguments>(arguments)...);
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/std_format_runtime_versus_compile_time_check_cpp23";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_FORMAT_HEADER && defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    LEARN_EXPECT_EQ(checks, checked_format("id={:04}", 27), std::string{"id=0027"});

    const std::string runtime_pattern{"{} + {} = {}"};
    int left = 2;
    int right = 3;
    int total = 5;
    LEARN_EXPECT_EQ(checks, std::vformat(runtime_pattern, std::make_format_args(left, right, total)),
                    std::string{"2 + 3 = 5"});

    const std::string invalid_runtime{"{:d}"};
    std::string text{"not-an-integer"};
    LEARN_EXPECT_THROWS(checks, std::format_error, std::vformat(invalid_runtime, std::make_format_args(text)));
    // In C++20/23, use vformat for runtime text. std::runtime_format belongs to a later standard revision.
#if 0
    const auto compile_time_error = std::format("{:d}", std::string{"text"});
#endif
#else
    LEARN_EXPECT_EQ(checks, std::string{"id="} + "0027", std::string{"id=0027"});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_format >= 201907L and <format>");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part3/section01/std_format_runtime_versus_compile_time_check_cpp23", run>;

}  // namespace
