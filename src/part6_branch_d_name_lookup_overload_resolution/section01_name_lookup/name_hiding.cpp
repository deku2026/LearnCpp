// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : name_hiding
// Topic id : part6/d/section01/name_hiding
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section01/name_hiding";

struct BaseParser {
    [[nodiscard]] constexpr int parse(int value) const { return value + 1; }
};

struct HidingParser : BaseParser {
    // This declaration hides every BaseParser overload named parse.
    [[nodiscard]] constexpr int parse(std::string_view text) const { return static_cast<int>(text.size()); }
};

struct CompleteParser : BaseParser {
    using BaseParser::parse;

    [[nodiscard]] constexpr int parse(std::string_view text) const { return static_cast<int>(text.size()); }
};

template <class Parser>
concept ParsesInt = requires(const Parser& parser) { parser.parse(7); };

static_assert(!ParsesInt<HidingParser>);
static_assert(ParsesInt<CompleteParser>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const CompleteParser parser;
    LEARN_EXPECT_EQ(checks, parser.parse(7), 8);
    LEARN_EXPECT_EQ(checks, parser.parse("lookup"), 6);

#if 0
    HidingParser hidden;
    hidden.parse(7);  // Ill-formed: the derived declaration hides BaseParser::parse.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section01/name_hiding", run>;

}  // namespace
