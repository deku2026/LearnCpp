// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : implicit_conversion_sequence_ranking
// Topic id : part6/d/section03/implicit_conversion_sequence_ranking
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section03/implicit_conversion_sequence_ranking";

enum class ConversionKind { standard, user_defined, ellipsis };

struct Box {
    constexpr Box(int input) : value(input) {}
    int value{};
};

[[nodiscard]] constexpr ConversionKind classify(long) {
    return ConversionKind::standard;
}
[[nodiscard]] constexpr ConversionKind classify(Box) {
    return ConversionKind::user_defined;
}
[[nodiscard]] constexpr ConversionKind classify(...) {
    return ConversionKind::ellipsis;
}

struct OnlyBox {
    constexpr operator Box() const { return Box{7}; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    // Standard conversion beats a user-defined conversion, which in turn
    // beats the ellipsis sequence.
    LEARN_EXPECT(checks, classify(7) == ConversionKind::standard);
    LEARN_EXPECT(checks, classify(Box{7}) == ConversionKind::user_defined);
    LEARN_EXPECT(checks, classify(OnlyBox{}) == ConversionKind::user_defined);

    struct NoConversion {};
    LEARN_EXPECT(checks, classify(NoConversion{}) == ConversionKind::ellipsis);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section03/implicit_conversion_sequence_ranking", run>;

}  // namespace
