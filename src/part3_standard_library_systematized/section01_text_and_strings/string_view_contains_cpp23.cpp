// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_view_contains_cpp23
// Topic id : part3/section01/string_view_contains_cpp23
// Reference: WG21 P1679R3 (__cpp_lib_string_contains).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<string_view>)
#include <string_view>
#define LEARN_HAS_STRING_VIEW_HEADER 1
#else
#define LEARN_HAS_STRING_VIEW_HEADER 0
#endif

#include <string>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/string_view_contains_cpp23";
    learn::ExampleChecks checks{topic};
    const std::string owner{"key=value;mode=safe"};
    const std::string_view view{owner};

#if LEARN_HAS_STRING_VIEW_HEADER && defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    LEARN_EXPECT(checks, view.contains("value"));
    LEARN_EXPECT(checks, view.contains('='));
    LEARN_EXPECT(checks, view.contains(std::string_view{}));
    LEARN_EXPECT(checks, !view.contains("Value"));
    LEARN_EXPECT(checks, view.substr(10).contains("safe"));
    // The query allocates nothing and does not extend `owner`'s lifetime.
#else
    LEARN_EXPECT(checks, view.find("value") != std::string_view::npos);
    LEARN_EXPECT(checks, view.find("Value") == std::string_view::npos);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_string_contains >= 202011L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_view_contains_cpp23", run>;

}  // namespace
