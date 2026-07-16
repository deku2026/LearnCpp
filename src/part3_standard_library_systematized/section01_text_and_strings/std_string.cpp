// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string
// Topic id : part3/section01/std_string
// Reference: C++ working draft [basic.string], [string.require].

#include "learn/example_support.hpp"

#if __has_include(<string>)
#include <string>
#define LEARN_HAS_STRING_HEADER 1
#else
#define LEARN_HAS_STRING_HEADER 0
#endif

#include <stdexcept>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/std_string";

#if LEARN_HAS_STRING_HEADER
    learn::ExampleChecks checks{topic};
    std::string greeting{"hello"};
    greeting += ", world";
    greeting.replace(0, 5, "Hello");
    LEARN_EXPECT_EQ(checks, greeting, std::string{"Hello, world"});
    LEARN_EXPECT_EQ(checks, greeting.substr(7), std::string{"world"});

    const std::string binary{"A\0B", 3};
    LEARN_EXPECT_EQ(checks, binary.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, binary[1], '\0');
    LEARN_EXPECT_THROWS(checks, std::out_of_range, greeting.at(999));

    const auto old_capacity = greeting.capacity();
    greeting.reserve(old_capacity + 64);
    LEARN_EXPECT(checks, greeting.capacity() >= old_capacity + 64);
    LEARN_EXPECT(checks, greeting.size() <= greeting.capacity());

    // string owns contiguous storage and appends are amortized O(1). Reallocation invalidates every pointer,
    // reference, iterator, and string_view into it; never use a pre-reserve iterator after the call above.
    // Small-string optimization is common but neither its existence nor its threshold is guaranteed.
#if 0
    const char unchecked = greeting[999];  // Out-of-range operator[] access is not bounds-checked.
#endif
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic, "standard <string> header");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string", run>;

}  // namespace
