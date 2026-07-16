// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_view
// Topic id : part3/section01/std_string_view
// Reference: C++ working draft [string.view].

#include "learn/example_support.hpp"

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
    constexpr auto topic = "part3/section01/std_string_view";

#if LEARN_HAS_STRING_VIEW_HEADER
    learn::ExampleChecks checks{topic};
    std::string owner{"prefix:value:suffix"};
    std::string_view view{owner};
    view.remove_prefix(7);
    view.remove_suffix(7);
    LEARN_EXPECT_EQ(checks, view, std::string_view{"value"});

    owner[7] = 'V';
    LEARN_EXPECT_EQ(checks, view, std::string_view{"Value"});
    LEARN_EXPECT_EQ(checks, view.substr(1, 3), std::string_view{"alu"});

    constexpr std::string_view binary{"A\0B", 3};
    LEARN_EXPECT_EQ(checks, binary.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, binary[1], '\0');

    // Copying and slicing a view are O(1). The source must outlive every use of the view, and data() is not
    // guaranteed to point at a null-terminated sequence after slicing.
#if 0
    std::string_view dangling() {
        std::string local{"destroyed"};
        return local;
    }
    std::printf("%s", view.data());  // Wrong for arbitrary slices and embedded nulls; pass an explicit length.
#endif
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic, "standard <string_view> header");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_view", run>;

}  // namespace
