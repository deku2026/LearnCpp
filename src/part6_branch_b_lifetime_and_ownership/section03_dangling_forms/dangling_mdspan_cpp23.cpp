// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_mdspan_cpp23
// Topic id : part6/b/section03/dangling_mdspan_cpp23
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#if __has_include(<mdspan>)
#include <mdspan>
#endif
#include <version>

namespace {

constexpr std::string_view kTopic = "part6/b/section03/dangling_mdspan_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    std::array<int, 6> owner{1, 2, 3, 4, 5, 6};
    std::mdspan<int, std::extents<std::size_t, 2, 3>> view{owner.data()};
    LEARN_EXPECT_EQ(checks, (view[1, 2]), 6);
    view[0, 1] = 9;
    LEARN_EXPECT_EQ(checks, owner[1], 9);
    // mdspan does not own its data; owner remains alive for the whole observation.
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::mdspan");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section03/dangling_mdspan_cpp23", run>;

}  // namespace
