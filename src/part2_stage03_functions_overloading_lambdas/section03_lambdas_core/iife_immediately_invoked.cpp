// An immediately invoked lambda can build one const value with local logic.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : iife_immediately_invoked
// Topic id : part2/stage03/section03/iife_immediately_invoked
//
// The temporary closure disappears after initialization; only its computed
// result remains.

#include "learn/example_support.hpp"

#include <string>
#include <vector>

namespace {

static_assert([] { return 6 * 7; }() == 42);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/iife_immediately_invoked"};

    const std::vector<int> primes_below_twenty = [] {
        std::vector<int> primes;
        for (int candidate = 2; candidate < 20; ++candidate) {
            bool prime = true;
            for (int divisor = 2; divisor * divisor <= candidate; ++divisor) {
                if (candidate % divisor == 0) {
                    prime = false;
                    break;
                }
            }
            if (prime) {
                primes.push_back(candidate);
            }
        }
        return primes;
    }();

    const std::string label = [count = primes_below_twenty.size()] { return "prime-count=" + std::to_string(count); }();

    LEARN_EXPECT_EQ(checks, primes_below_twenty, std::vector<int>({2, 3, 5, 7, 11, 13, 17, 19}));
    LEARN_EXPECT_EQ(checks, label, std::string{"prime-count=8"});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/iife_immediately_invoked", run>;

}  // namespace
