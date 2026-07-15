// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_pack_cpp20
// Topic id : part2/stage03/section03/capture_pack_cpp20
//
// Covers: C++20 pack capture [...xs] / [&...xs]

#include "learn/topic_registry.hpp"

#include <tuple>
#include <utility>

namespace {

template <class... Args>
auto make_sum_by_value(Args... args) {
    return [... args = std::move(args)] { return (0 + ... + args); };
}

template <class... Args>
auto make_sum_by_ref(Args&... args) {
    return [&args...]() -> int { return (0 + ... + args); };
}

template <class... Args>
auto make_tuple_holder(Args... args) {
    return [... args = std::move(args)] { return std::make_tuple(args...); };
}

void demo_basics() {
    auto f = make_sum_by_value(1, 2, 3);
    LEARN_CHECK(f() == 6);
}

void demo_intermediate() {
    int a = 10;
    int b = 20;
    auto f = make_sum_by_ref(a, b);
    a = 1;
    b = 2;
    LEARN_CHECK(f() == 3);
}

void demo_expert() {
    auto holder = make_tuple_holder(4, 5);
    auto t = holder();
    LEARN_CHECK(std::get<0>(t) == 4);
    LEARN_CHECK(std::get<1>(t) == 5);

    auto empty = make_sum_by_value();
    LEARN_CHECK(empty() == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_pack_cpp20", run>;

}  // namespace
