// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : asan_runtime_safety_net
// Topic id : part6/b/section06/asan_runtime_safety_net
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <ranges>
#include <span>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section06/asan_runtime_safety_net";

constexpr bool enabled =
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
    true;
#else
    false;
#endif
#elif defined(__SANITIZE_ADDRESS__)
    true;
#else
    false;
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr std::array values{1, 2, 3};
    const std::span view{values};
    LEARN_EXPECT_EQ(checks, view.back(), 3);
    if (enabled) LEARN_EXPECT(checks, enabled);
    // ASan detects many executed memory bugs; it neither proves safety nor defines invalid operations.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section06/asan_runtime_safety_net", run>;

}  // namespace
