// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : best_viable_function
// Topic id : part6/d/section03/best_viable_function
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section03/best_viable_function";

enum class Rank { exact_int, promoted_double, converted_long };

[[nodiscard]] constexpr Rank rank(int) {
    return Rank::exact_int;
}
[[nodiscard]] constexpr Rank rank(double) {
    return Rank::promoted_double;
}
[[nodiscard]] constexpr Rank rank(long) {
    return Rank::converted_long;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT(checks, rank(1) == Rank::exact_int);
    const short small = 2;
    LEARN_EXPECT(checks, rank(small) == Rank::exact_int);  // integral promotion
    const float real = 2.5F;
    LEARN_EXPECT(checks, rank(real) == Rank::promoted_double);  // floating promotion
    LEARN_EXPECT(checks, rank(3L) == Rank::converted_long);

#if 0
    void ambiguous(long);
    void ambiguous(double);
    ambiguous(1);  // Both conversions have equal rank: no unique best function.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section03/best_viable_function", run>;

}  // namespace
