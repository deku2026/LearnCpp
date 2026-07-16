// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : using_introduction
// Topic id : part6/d/section01/using_introduction
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section01/using_introduction";

namespace source_api {

enum class Status { idle, ready };

[[nodiscard]] constexpr int transform(int value) {
    return value * 2;
}
[[nodiscard]] constexpr long transform(long value) {
    return value * 3;
}

}  // namespace source_api

namespace facade {

// A using-declaration introduces this overload set without making every name
// from source_api visible.
using source_api::transform;
using enum source_api::Status;

}  // namespace facade

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, facade::transform(3), 6);
    LEARN_EXPECT_EQ(checks, facade::transform(3L), 9L);
    LEARN_EXPECT(checks, facade::ready == source_api::Status::ready);

    // A block-scope using-declaration is often preferable to a using-directive.
    using source_api::transform;
    LEARN_EXPECT_EQ(checks, transform(4), 8);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section01/using_introduction", run>;

}  // namespace
