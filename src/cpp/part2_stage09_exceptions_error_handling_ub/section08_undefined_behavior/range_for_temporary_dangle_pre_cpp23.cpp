// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : range_for_temporary_dangle_pre_cpp23
// Topic id : part2/stage09/section08/range_for_temporary_dangle_pre_cpp23
//
// Covers: pre-C++23 temporary lifetime pitfalls in range-for — safe patterns

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace {

std::vector<std::string> make_words() {
    return {"a", "bb", "ccc"};
}

// Hypothetical problematic pattern (NOT executed as dangling):
// for (auto& c : make_words()[0]) ... depending on binding details.
// SAFE approach: name the temporary.

void demo_basics() {
    int n = 0;
    for (const auto& w : make_words()) {
        n += static_cast<int>(w.size());
    }
    assert(n == 6);
}

void demo_intermediate() {
    // C++20 init-statement keeps temporary alive for the loop.
    std::size_t total = 0;
    for (auto words = make_words(); const auto& w : words) {
        total += w.size();
    }
    assert(total == 6);
}

void demo_expert() {
    // SAFE: copy element then iterate characters.
    auto words = make_words();
    int letters = 0;
    for (char c : words[2]) {
        (void)c;
        ++letters;
    }
    assert(letters == 3);

    // Comment only: chaining temporary containers without lifetime extension
    // was a pre-C++23 footgun in some patterns; keep a named owner.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/range_for_temporary_dangle_pre_cpp23", run>;

}  // namespace
