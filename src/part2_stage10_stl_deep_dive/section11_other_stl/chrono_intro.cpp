// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : chrono_intro
// Topic id : part2/stage10/section11/chrono_intro
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <chrono>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/chrono_intro";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    using namespace std::chrono_literals;
    constexpr auto delay = 2s + 250ms;
    static_assert(std::chrono::duration_cast<std::chrono::milliseconds>(delay).count() == 2'250);
    const std::chrono::steady_clock::time_point start{10s};
    const auto finish = start + delay;
    LEARN_EXPECT_EQ(checks, std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count(), 2'250LL);
    LEARN_EXPECT(checks, std::chrono::steady_clock::is_steady);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section11/chrono_intro", run>;

}  // namespace
