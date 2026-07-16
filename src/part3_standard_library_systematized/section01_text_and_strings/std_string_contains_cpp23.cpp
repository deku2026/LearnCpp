// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_contains_cpp23
// Topic id : part3/section01/std_string_contains_cpp23
// Reference: WG21 P1679R3 (__cpp_lib_string_contains).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<string>)
#include <string>
#define LEARN_HAS_STRING_HEADER 1
#else
#define LEARN_HAS_STRING_HEADER 0
#endif

#include <string_view>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/std_string_contains_cpp23";
    learn::ExampleChecks checks{topic};
    const std::string text{"alpha\0omega", 11};

#if LEARN_HAS_STRING_HEADER && defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    LEARN_EXPECT(checks, text.contains("alpha"));
    LEARN_EXPECT(checks, text.contains(std::string_view{"\0om", 3}));
    LEARN_EXPECT(checks, text.contains('\0'));
    LEARN_EXPECT(checks, text.contains(std::string_view{}));
    LEARN_EXPECT(checks, !text.contains("ALPHA"));
    LEARN_EXPECT(checks, !text.contains("missing"));
    // contains is equivalent to find(...) != npos: byte-based, case-sensitive, and generally O(N*M).
#else
    LEARN_EXPECT(checks, text.find("alpha") != std::string::npos);
    LEARN_EXPECT(checks, text.find("missing") == std::string::npos);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_string_contains >= 202011L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_contains_cpp23", run>;

}  // namespace
