// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : use_after_free_deep
// Topic id : part6/g/section03/use_after_free_deep
//
// Covers: use-after-free is UB; reset pointers and prefer RAII

#include "learn/topic_registry.hpp"

#include <memory>

namespace {

void demo_basics() {
    auto p = std::make_unique<int>(3);
    LEARN_CHECK(*p == 3);
    p.reset();
    LEARN_CHECK(p == nullptr);
}

void demo_intermediate() {
    int* raw = new int(4);
    LEARN_CHECK(*raw == 4);
    delete raw;
    raw = nullptr;
    LEARN_CHECK(raw == nullptr);
}

void demo_expert() {
    // Not executed: use after delete.
    std::shared_ptr<int> a = std::make_shared<int>(1);
    std::weak_ptr<int> w = a;
    a.reset();
    LEARN_CHECK(w.expired());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/use_after_free_deep", run>;

}  // namespace
