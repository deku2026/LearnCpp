// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_compare_and_locale_pitfalls
// Topic id : part3/section01/string_compare_and_locale_pitfalls
// Reference: C++ working draft [string.classes], [locale], [cctype.syn].

#include "learn/example_support.hpp"

#if __has_include(<locale>)
#include <locale>
#define LEARN_HAS_LOCALE_HEADER 1
#else
#define LEARN_HAS_LOCALE_HEADER 0
#endif

#include <cctype>
#include <string>
#include <string_view>

namespace {

std::string ascii_lower(std::string_view text) {
    std::string result{text};
    for (char& character : result) {
        if (character >= 'A' && character <= 'Z') {
            character = static_cast<char>(character - 'A' + 'a');
        }
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/string_compare_and_locale_pitfalls";
    learn::ExampleChecks checks{topic};

    LEARN_EXPECT(checks, std::string{"alpha"} < std::string{"alphabet"});
    LEARN_EXPECT(checks, std::string{"File"} != std::string{"file"});
    LEARN_EXPECT_EQ(checks, ascii_lower("ASCII-ONLY"), std::string{"ascii-only"});

    const std::string first{"a\0b", 3};
    const std::string second{"a\0c", 3};
    LEARN_EXPECT(checks, first < second);

#if LEARN_HAS_LOCALE_HEADER
    const auto& collation = std::use_facet<std::collate<char>>(std::locale::classic());
    const std::string left{"abc"};
    const std::string right{"abd"};
    LEARN_EXPECT(checks, collation.compare(left.data(), left.data() + left.size(), right.data(),
                                           right.data() + right.size()) < 0);
#endif
    LEARN_EXPECT_EQ(checks, static_cast<char>(std::tolower(static_cast<unsigned char>('A'))), 'a');
    // std::string compares code units, not human language. UTF normalization, case folding, collation, and
    // grapheme handling require a Unicode library such as ICU; locale names are also platform-specific.
#if 0
    const char possibly_negative = '\xFF';
    const int undefined = std::tolower(possibly_negative);  // Cast to unsigned char before cctype calls.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_compare_and_locale_pitfalls", run>;

}  // namespace
