// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : static_operator_call_cpp23
// Topic id : part2/stage03/section04/static_operator_call_cpp23
//
// Covers: C++23 static call operator on capture-less lambdas

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <vector>

namespace {

void demo_basics() {
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    auto sq = [](int x) static { return x * x; };
    LEARN_CHECK(sq(5) == 25);
#else
    auto sq = [](int x) { return x * x; };
    LEARN_CHECK(sq(5) == 25);
#endif
}

void demo_intermediate() {
    std::vector<int> v{3, 1, 2};
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    std::sort(v.begin(), v.end(), [](int a, int b) static { return a < b; });
#else
    std::sort(v.begin(), v.end(), [](int a, int b) { return a < b; });
#endif
    LEARN_CHECK(v[0] == 1 && v[1] == 2 && v[2] == 3);
}

void demo_expert() {
    // static lambda cannot capture: no this for data members.
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    auto id = [](int x) static { return x; };
#else
    auto id = [](int x) { return x; };
#endif
    LEARN_CHECK(id(9) == 9);

    int n = 1;
    auto with_capture = [n](int x) { return x + n; };
    LEARN_CHECK(with_capture(2) == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/static_operator_call_cpp23", run>;

}  // namespace
