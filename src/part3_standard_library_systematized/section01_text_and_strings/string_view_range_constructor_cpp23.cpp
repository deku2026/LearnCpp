// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_view_range_constructor_cpp23
// Topic id : part3/section01/string_view_range_constructor_cpp23
// Reference: WG21 P1989R2 and P2499R0. No dedicated SD-6 feature-test macro was adopted.

#include "learn/example_support.hpp"

#if __has_include(<string_view>)
#include <string_view>
#define LEARN_HAS_STRING_VIEW_HEADER 1
#else
#define LEARN_HAS_STRING_VIEW_HEADER 0
#endif

#include <array>
#include <concepts>
#include <list>

namespace {

#if LEARN_HAS_STRING_VIEW_HEADER
template <class Range>
int demonstrate_range_constructor(Range& characters) {
    constexpr auto topic = "part3/section01/string_view_range_constructor_cpp23";
    learn::ExampleChecks checks{topic};

    if constexpr (requires { std::string_view{characters}; }) {
        const std::string_view view{characters};
        LEARN_EXPECT_EQ(checks, view.size(), characters.size());
        LEARN_EXPECT_EQ(checks, view.front(), 'A');
        LEARN_EXPECT_EQ(checks, view[2], '\0');
        characters[1] = 'Z';
        LEARN_EXPECT_EQ(checks, view[1], 'Z');
        // The constructor is explicit and non-owning. The range must be contiguous, sized, and have char values.
#if 0
        const std::string_view not_contiguous{std::list<char>{'a', 'b'}};
        const std::string_view dangling{std::array{'x', 'y'}};
#endif
        return checks.result();
    } else {
        LEARN_EXPECT_EQ(checks, characters.size(), std::size_t{5});
        if (const int result = checks.result(); result != 0) {
            return result;
        }
        return learn::ExampleChecks::unavailable(topic, "C++23 basic_string_view contiguous-range constructor");
    }
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if LEARN_HAS_STRING_VIEW_HEADER && __cplusplus >= 202302L
    std::array characters{'A', 'B', '\0', 'C', 'D'};
    return demonstrate_range_constructor(characters);
#else
    constexpr auto topic = "part3/section01/string_view_range_constructor_cpp23";
    return learn::ExampleChecks::unavailable(topic, "C++23 <string_view> range construction");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_view_range_constructor_cpp23", run>;

}  // namespace
