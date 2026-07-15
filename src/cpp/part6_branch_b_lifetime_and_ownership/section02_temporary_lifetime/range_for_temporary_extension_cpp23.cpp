// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : range_for_temporary_extension_cpp23
// Topic id : part6/b/section02/range_for_temporary_extension_cpp23
//
// Covers: range-for temporary range extension; C++23 expands some cases

#include "learn/topic_registry.hpp"

#include <vector>

namespace {

std::vector<int> make_vec() {
    return {1, 2, 3};
}

void demo_basics() {
    int sum = 0;
    for (int x : make_vec()) {
        sum += x;
    }
    LEARN_CHECK(sum == 6);
}

void demo_intermediate() {
    // Binding the range to a reference keeps the temporary alive for the loop.
    int product = 1;
    for (int x : make_vec()) {
        product *= x;
    }
    LEARN_CHECK(product == 6);
}

void demo_expert() {
#if defined(__cpp_range_based_for) || 1
    // Teaching: C++23 addresses some lifetime gaps for range-for initializers;
    // still avoid returning references into temporaries from helpers.
    const auto& range = make_vec();
    LEARN_CHECK(range.size() == 3);
    LEARN_CHECK(range[0] == 1);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/range_for_temporary_extension_cpp23", run>;

}  // namespace
