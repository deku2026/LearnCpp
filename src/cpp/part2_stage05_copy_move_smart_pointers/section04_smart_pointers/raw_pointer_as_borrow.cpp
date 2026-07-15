// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : raw_pointer_as_borrow
// Topic id : part2/stage05/section04/raw_pointer_as_borrow
//
// Covers: raw T* means borrow/observer; smart pointers express ownership

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <utility>

namespace {

int observe(const int* p) {
    // Borrow: may be null; never delete.
    if (!p) {
        return -1;
    }
    return *p;
}

int observe_ref(const int& r) {
    // Non-null borrow.
    return r;
}

void take_unique(std::unique_ptr<int> p) {
    assert(p != nullptr);
    assert(*p == 42);
}

void share(std::shared_ptr<int> p) {
    assert(p.use_count() >= 1);
    assert(*p == 7);
}

void demo_basics() {
    auto owner = std::make_unique<int>(42);
    assert(observe(owner.get()) == 42);
    assert(owner != nullptr);  // still owns after borrow
    assert(observe(nullptr) == -1);
}

void demo_intermediate() {
    auto owner = std::make_unique<int>(42);
    take_unique(std::move(owner));  // ownership transfer
    assert(owner == nullptr);

    auto shared = std::make_shared<int>(7);
    share(shared);  // shared ownership copy
    assert(shared.use_count() == 1);
    assert(*shared == 7);
}

void demo_expert() {
    int stack = 11;
    assert(observe(&stack) == 11);
    assert(observe_ref(stack) == 11);

    // API design sketch:
    //  - take ownership: unique_ptr by value
    //  - share ownership: shared_ptr by value
    //  - borrow only: T* / const T&
    auto factory = []() { return std::make_unique<int>(99); };
    auto p = factory();
    const int* borrowed = p.get();
    assert(observe(borrowed) == 99);
    assert(p != nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/raw_pointer_as_borrow", run>;

}  // namespace
