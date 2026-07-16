// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : qualified_lookup
// Topic id : part6/d/section01/qualified_lookup
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section01/qualified_lookup";

namespace qualified_demo {

constexpr int version = 1;

namespace api {
constexpr int version = 2;

struct Settings {
    static constexpr int retries = 3;
};
}  // namespace api

}  // namespace qualified_demo

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    // Each qualifier fixes the scope in which the next component is looked up.
    LEARN_EXPECT_EQ(checks, qualified_demo::version, 1);
    LEARN_EXPECT_EQ(checks, qualified_demo::api::version, 2);
    LEARN_EXPECT_EQ(checks, qualified_demo::api::Settings::retries, 3);

    namespace selected_api = qualified_demo::api;
    LEARN_EXPECT_EQ(checks, selected_api::Settings::retries, 3);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section01/qualified_lookup", run>;

}  // namespace
