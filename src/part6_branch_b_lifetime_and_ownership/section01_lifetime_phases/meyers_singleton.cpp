// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : meyers_singleton
// Topic id : part6/b/section01/meyers_singleton
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/b/section01/meyers_singleton";

class Settings {
public:
    static Settings& instance() {
        static Settings value;
        return value;
    }
    int token() const noexcept { return token_; }

private:
    Settings() = default;
    int token_{23};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::array<Settings*, 4> addresses{};
    std::array<std::jthread, 4> threads;
    for (std::size_t index = 0; index < threads.size(); ++index) {
        threads[index] = std::jthread{[&, index] { addresses[index] = &Settings::instance(); }};
    }
    for (auto& thread : threads) thread.join();
    LEARN_EXPECT(checks, std::ranges::all_of(addresses, [&](Settings* value) { return value == addresses[0]; }));
    LEARN_EXPECT_EQ(checks, addresses[0]->token(), 23);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section01/meyers_singleton", run>;

}  // namespace
