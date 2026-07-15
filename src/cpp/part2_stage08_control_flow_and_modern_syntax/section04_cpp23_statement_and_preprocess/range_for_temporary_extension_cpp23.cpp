// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : range_for_temporary_extension_cpp23
// Topic id : part2/stage08/section04/range_for_temporary_extension_cpp23
//
// Covers: C++23 range-for temporary lifetime extension (safe patterns)

#include "learn/topic_registry.hpp"

#include <string>
#include <vector>

namespace {

std::vector<int> make_vec() {
    return {1, 2, 3, 4};
}

std::string make_str() {
    return "abcd";
}

void demo_basics() {
    int sum = 0;
    // Temporary from make_vec() is bound for the whole loop (C++11 already for range expr).
    for (int x : make_vec()) {
        sum += x;
    }
    LEARN_CHECK(sum == 10);
}

void demo_intermediate() {
    // C++20 init-statement keeps named temporary clearly in scope.
    std::size_t n = 0;
    for (auto s = make_str(); char c : s) {
        (void)c;
        ++n;
    }
    LEARN_CHECK(n == 4);
}

void demo_expert() {
    // SAFE: avoid dangling by not taking pointers into temporary after loop.
    int last = 0;
    for (int x : make_vec()) {
        last = x;
    }
    LEARN_CHECK(last == 4);

    // Prefer init-statement when chaining factories.
    int total = 0;
    for (const auto& v = make_vec(); int x : v) {
        total += x;
    }
    LEARN_CHECK(total == 10);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section04/range_for_temporary_extension_cpp23", run>;

}  // namespace
