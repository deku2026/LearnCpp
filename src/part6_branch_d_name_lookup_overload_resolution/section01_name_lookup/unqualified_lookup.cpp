// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : unqualified_lookup
// Topic id : part6/d/section01/unqualified_lookup
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section01/unqualified_lookup";

namespace lookup_demo {

constexpr int value = 10;

constexpr int nearest_scope_wins() {
    const int value = 20;
    {
        const int innermost = value + 2;
        return innermost;
    }
}

struct Counter {
    int value = 30;

    [[nodiscard]] constexpr int read() const {
        // Unqualified lookup first finds the data member in class scope.
        return value;
    }
};

}  // namespace lookup_demo

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, lookup_demo::value, 10);
    LEARN_EXPECT_EQ(checks, lookup_demo::nearest_scope_wins(), 22);
    LEARN_EXPECT_EQ(checks, lookup_demo::Counter{}.read(), 30);

    // A declaration stops the outward search at the first matching scope.
    const int value = 40;
    LEARN_EXPECT_EQ(checks, value, 40);
    LEARN_EXPECT_EQ(checks, lookup_demo::value, 10);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section01/unqualified_lookup", run>;

}  // namespace
