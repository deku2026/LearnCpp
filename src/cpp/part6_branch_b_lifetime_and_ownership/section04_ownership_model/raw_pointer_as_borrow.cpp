// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : raw_pointer_as_borrow
// Topic id : part6/b/section04/raw_pointer_as_borrow
//
// Covers: raw pointer as non-owning borrow; owner remains unique_ptr/shared_ptr

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>

namespace {

void use(const int* borrowed) {
    assert(borrowed != nullptr);
    assert(*borrowed == 42);
}

void demo_basics() {
    auto owner = std::make_unique<int>(42);
    use(owner.get());
    assert(*owner == 42);
}

void demo_intermediate() {
    int stack = 7;
    const int* borrow = &stack;
    assert(*borrow == 7);
}

void demo_expert() {
    // Document ownership in API: raw* means non-owning; unique_ptr transfers ownership.
    std::unique_ptr<int> owner = std::make_unique<int>(1);
    int* borrow = owner.get();
    assert(borrow == owner.get());
    assert(*borrow == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/raw_pointer_as_borrow", run>;

}  // namespace
