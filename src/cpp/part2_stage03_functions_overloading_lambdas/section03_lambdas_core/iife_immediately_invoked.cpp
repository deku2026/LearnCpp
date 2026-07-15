// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : iife_immediately_invoked
// Topic id : part2/stage03/section03/iife_immediately_invoked
//
// Covers: immediately-invoked lambda for const init / scoped logic

#include "learn/topic_registry.hpp"

#include <string>
#include <vector>

namespace {

void demo_basics() {
    const int x = [] { return 40 + 2; }();
    LEARN_CHECK(x == 42);
}

void demo_intermediate() {
    const std::vector<int> primes = [] {
        std::vector<int> v;
        for (int n = 2; n < 20; ++n) {
            bool is_prime = true;
            for (int d = 2; d * d <= n; ++d) {
                if (n % d == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                v.push_back(n);
            }
        }
        return v;
    }();
    LEARN_CHECK(primes.front() == 2);
    LEARN_CHECK(primes.back() == 19);
    LEARN_CHECK(primes.size() == 8);
}

void demo_expert() {
    const std::string label = [](int code) {
        if (code == 0) {
            return std::string{"ok"};
        }
        return std::string{"err"};
    }(0);
    LEARN_CHECK(label == "ok");

    int side = 0;
    const int y = [&side] {
        side = 1;
        return 9;
    }();
    LEARN_CHECK(y == 9 && side == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/iife_immediately_invoked", run>;

}  // namespace
