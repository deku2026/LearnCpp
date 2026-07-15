// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : shared_unique_weak_revisit
// Topic id : part6/b/section04/shared_unique_weak_revisit
//
// Covers: unique exclusive, shared shared-ownership, weak non-owning observe

#include "learn/topic_registry.hpp"

#include <memory>

namespace {

void demo_basics() {
    auto u = std::make_unique<int>(3);
    LEARN_CHECK(*u == 3);
    auto s = std::make_shared<int>(4);
    LEARN_CHECK(*s == 4);
    LEARN_CHECK(s.use_count() == 1);
}

void demo_intermediate() {
    auto s = std::make_shared<int>(10);
    std::weak_ptr<int> w = s;
    LEARN_CHECK(!w.expired());
    if (auto locked = w.lock()) {
        LEARN_CHECK(*locked == 10);
    }
    s.reset();
    LEARN_CHECK(w.expired());
}

void demo_expert() {
    auto s1 = std::make_shared<int>(1);
    auto s2 = s1;
    LEARN_CHECK(s1.use_count() == 2);
    std::unique_ptr<int> u = std::make_unique<int>(2);
    std::shared_ptr<int> from_u = std::move(u);
    LEARN_CHECK(!u);
    LEARN_CHECK(*from_u == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/shared_unique_weak_revisit", run>;

}  // namespace
