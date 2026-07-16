// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section09_constexpr_and_tmp_fusion
// Item     : migrate_tmp_to_constexpr
// Topic id : part6/e/section09/migrate_tmp_to_constexpr

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <type_traits>

namespace {

template <unsigned int N>
struct classic_factorial : std::integral_constant<unsigned long long, N * classic_factorial<N - 1>::value> {};

template <>
struct classic_factorial<0> : std::integral_constant<unsigned long long, 1> {};

[[nodiscard]] constexpr unsigned long long factorial(unsigned int value) noexcept {
    unsigned long long result = 1;
    for (unsigned int factor = 2; factor <= value; ++factor) {
        result *= factor;
    }
    return result;
}

template <std::size_t Limit>
[[nodiscard]] consteval auto prime_table() {
    std::array<bool, Limit> prime{};
    prime.fill(true);
    if constexpr (Limit > 0) {
        prime[0] = false;
    }
    if constexpr (Limit > 1) {
        prime[1] = false;
    }
    for (std::size_t candidate = 2; candidate * candidate < Limit; ++candidate) {
        if (prime[candidate]) {
            for (std::size_t composite = candidate * candidate; composite < Limit; composite += candidate) {
                prime[composite] = false;
            }
        }
    }
    return prime;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section09/migrate_tmp_to_constexpr"};

    static_assert(classic_factorial<10>::value == factorial(10));
    constexpr auto primes = prime_table<20>();
    static_assert(primes[2] && primes[3] && primes[19]);
    static_assert(!primes[0] && !primes[1] && !primes[15]);
    LEARN_EXPECT_EQ(checks, factorial(6), 720U);
    LEARN_EXPECT(checks, primes[17]);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section09/migrate_tmp_to_constexpr", run>;

}  // namespace
