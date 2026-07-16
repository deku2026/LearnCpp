// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_resize_and_overwrite_cpp23
// Topic id : part3/section01/std_string_resize_and_overwrite_cpp23
// Reference: WG21 P1072R10 (__cpp_lib_string_resize_and_overwrite).

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

#include <algorithm>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/std_string_resize_and_overwrite_cpp23";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_STRING_HEADER && defined(__cpp_lib_string_resize_and_overwrite) && \
    __cpp_lib_string_resize_and_overwrite >= 202110L
    std::string generated;
    generated.resize_and_overwrite(8, [](char* storage, std::size_t available) {
        const auto used = (std::min)(available, std::size_t{5});
        for (std::size_t index = 0; index < used; ++index) {
            storage[index] = static_cast<char>('A' + index);
        }
        return used;
    });
    LEARN_EXPECT_EQ(checks, generated, std::string{"ABCDE"});
    LEARN_EXPECT_EQ(checks, generated.size(), std::size_t{5});

    generated.resize_and_overwrite(16, [](char* storage, std::size_t available) {
        constexpr std::string_view replacement{"xyz"};
        if (available >= replacement.size()) {
            std::ranges::copy(replacement, storage);
            return replacement.size();
        }
        return std::size_t{0};
    });
    LEARN_EXPECT_EQ(checks, generated, std::string{"xyz"});
    // The callback writes directly into writable storage and returns the final size. Existing views and
    // iterators must be treated as invalidated; returning more than `available` violates the precondition.
#if 0
    generated.resize_and_overwrite(4, [](char*, std::size_t available) { return available + 1; });
#endif
#else
    std::string modeled(5, '\0');
    for (std::size_t index = 0; index < modeled.size(); ++index) {
        modeled[index] = static_cast<char>('A' + index);
    }
    LEARN_EXPECT_EQ(checks, modeled, std::string{"ABCDE"});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_string_resize_and_overwrite >= 202110L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_resize_and_overwrite_cpp23", run>;

}  // namespace
