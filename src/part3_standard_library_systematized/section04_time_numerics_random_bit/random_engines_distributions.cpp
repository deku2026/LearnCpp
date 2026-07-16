// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : random_engines_distributions
// Topic id : part3/section04/random_engines_distributions
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <array>
#include <concepts>
#include <random>
#include <sstream>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section04/random_engines_distributions";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::mt19937 engine{2026U};
    std::ostringstream saved;
    saved << engine;
    const auto first = engine();
    std::mt19937 restored;
    std::istringstream input{saved.str()};
    input >> restored;
    LEARN_EXPECT_EQ(checks, restored(), first);

    std::uniform_int_distribution<int> die{1, 6};
    std::array<int, 6> counts{};
    for (int roll = 0; roll < 600; ++roll) ++counts[static_cast<std::size_t>(die(engine) - 1)];
    int total{};
    for (const int count : counts) {
        LEARN_EXPECT(checks, count > 0);
        total += count;
    }
    LEARN_EXPECT_EQ(checks, total, 600);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/random_engines_distributions", run>;

}  // namespace
