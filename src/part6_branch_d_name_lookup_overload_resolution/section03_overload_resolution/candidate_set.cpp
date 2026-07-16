// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : candidate_set
// Topic id : part6/d/section03/candidate_set
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section03/candidate_set";

enum class Selected { ordinary_conversion, adl_exact };

namespace model {

struct Number {
    int value{};
    constexpr operator int() const noexcept { return value; }
};

[[nodiscard]] constexpr Selected select(Number) {
    return Selected::adl_exact;
}

}  // namespace model

[[nodiscard]] constexpr Selected select(int) {
    return Selected::ordinary_conversion;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    // Ordinary lookup contributes select(int); ADL contributes model::select.
    // Both are candidates, but the exact match is better than conversion to int.
    LEARN_EXPECT(checks, select(model::Number{3}) == Selected::adl_exact);
    LEARN_EXPECT(checks, select(3) == Selected::ordinary_conversion);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section03/candidate_set", run>;

}  // namespace
