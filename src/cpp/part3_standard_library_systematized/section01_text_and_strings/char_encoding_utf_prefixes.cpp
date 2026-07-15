// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : char_encoding_utf_prefixes
// Topic id : part3/section01/char_encoding_utf_prefixes
//
// Covers: char/char8_t/char16_t/char32_t/wchar_t and u8/u/U/L prefixes

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>

namespace {

void demo_basics() {
    const char* a = "abc";
    const char16_t* u = u"abc";
    const char32_t* U = U"abc";
    const wchar_t* w = L"abc";
    LEARN_CHECK(a[0] == 'a');
    LEARN_CHECK(u[0] == u'a');
    LEARN_CHECK(U[0] == U'a');
    LEARN_CHECK(w[0] == L'a');
}

void demo_intermediate() {
    const char8_t* u8 = u8"UTF-8";
    LEARN_CHECK(u8[0] == u8'U');
    static_assert(sizeof(char16_t) == 2);
    static_assert(sizeof(char32_t) == 4);
}

void demo_expert() {
    // bytes != code points for multi-byte UTF-8
    std::string s = "A";
    LEARN_CHECK(s.size() == 1);
    static_assert(std::is_same_v<decltype(u8"x"), const char8_t (&)[2]> ||
                  std::is_array_v<std::remove_reference_t<decltype(u8"x")>>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/char_encoding_utf_prefixes", run>;

}  // namespace
