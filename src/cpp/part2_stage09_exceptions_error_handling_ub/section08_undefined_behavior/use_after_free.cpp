// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : use_after_free
// Topic id : part2/stage09/section08/use_after_free
//
// Covers: use-after-free is UB — ownership patterns that prevent it

#include "learn/topic_registry.hpp"

#include <memory>
#include <string>

namespace {

void demo_basics() {
    // SAFE: unique_ptr owns; no manual delete + use.
    auto p = std::make_unique<int>(42);
    LEARN_CHECK(*p == 42);
    p.reset();
    LEARN_CHECK(p == nullptr);
    // Comment only: use of deleted memory is UB — we don't do it.
}

void demo_intermediate() {
    std::shared_ptr<std::string> a = std::make_shared<std::string>("hi");
    std::shared_ptr<std::string> b = a;
    LEARN_CHECK(a.use_count() == 2);
    a.reset();
    LEARN_CHECK(*b == "hi");  // still alive
}

void demo_expert() {
    // Dangling raw pointer risk: keep owner alive while observing.
    auto owner = std::make_unique<int>(7);
    int* observe = owner.get();
    LEARN_CHECK(observe != nullptr);
    LEARN_CHECK(*observe == 7);
    owner.reset();
    observe = nullptr;  // scrub; never dereference after free
    LEARN_CHECK(observe == nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/use_after_free", run>;

}  // namespace
