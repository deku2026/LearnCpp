// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : shared_ptr_control_block
// Topic id : part2/stage05/section04/shared_ptr_control_block
//
// Covers: control block holds strong/weak counts; one object one control block

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>

namespace {

void demo_basics() {
    auto a = std::make_shared<int>(42);
    assert(a.use_count() == 1);
    {
        auto b = a;
        assert(a.use_count() == 2);
        assert(b.use_count() == 2);
        assert(a.get() == b.get());
    }
    assert(a.use_count() == 1);
}

void demo_intermediate() {
    auto p = std::make_shared<int>(7);
    std::weak_ptr<int> w = p;
    assert(p.use_count() == 1);  // weak does not increase strong count
    assert(!w.expired());

    p.reset();
    assert(w.expired());
    auto locked = w.lock();
    assert(locked == nullptr);
}

void demo_expert() {
    // Never create two shared_ptr from the same raw pointer (would be two control blocks).
    auto a = std::make_shared<int>(1);
    auto b = a;  // correct: share control block
    assert(a.use_count() == 2);
    assert(a.get() == b.get());

    // Deleter type-erasure lives in the control block.
    int deleted = 0;
    {
        std::shared_ptr<int> p(new int(5), [&](int* raw) {
            ++deleted;
            delete raw;
        });
        assert(*p == 5);
        assert(p.use_count() == 1);
    }
    assert(deleted == 1);

    // Owner_before compares control blocks (stable ownership identity).
    auto x = std::make_shared<int>(1);
    auto y = std::make_shared<int>(2);
    const bool ordered = x.owner_before(y) || y.owner_before(x);
    assert(ordered);  // distinct control blocks are ordered one way or the other
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/shared_ptr_control_block", run>;

}  // namespace
