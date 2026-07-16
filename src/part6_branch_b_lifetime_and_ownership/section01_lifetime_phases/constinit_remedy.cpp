// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : constinit_remedy
// Topic id : part6/b/section01/constinit_remedy
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <atomic>
#include <ranges>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/b/section01/constinit_remedy";

constinit int port = 8080;
constinit std::atomic<int> requests{0};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, port, 8080);
    port = 9090;  // constinit constrains initialization, not later mutation or constness.
    LEARN_EXPECT_EQ(checks, port, 9090);
    requests.fetch_add(1, std::memory_order_relaxed);
    LEARN_EXPECT_EQ(checks, requests.load(std::memory_order_relaxed), 1);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section01/constinit_remedy", run>;

}  // namespace
