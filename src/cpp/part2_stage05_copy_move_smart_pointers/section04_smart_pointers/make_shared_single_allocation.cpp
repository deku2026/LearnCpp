// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : make_shared_single_allocation
// Topic id : part2/stage05/section04/make_shared_single_allocation
//
// Covers: make_shared usually one allocation (object + control block together)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>

namespace {

struct Counted {
    int value;
    explicit Counted(int v) : value(v) {}
};

void demo_basics() {
    auto p = std::make_shared<int>(42);
    assert(p.use_count() == 1);
    assert(*p == 42);
}

void demo_intermediate() {
    // make_shared: typically one heap allocation (object + control block together).
    // shared_ptr(new T): two allocations (object, then control block).
    auto a = std::make_shared<Counted>(1);
    auto b = std::shared_ptr<Counted>(new Counted(2));
    assert(a->value == 1);
    assert(b->value == 2);
    assert(a.use_count() == 1);
    assert(b.use_count() == 1);
    assert(a.get() != b.get());
}

void demo_expert() {
    // Exception-safety / ergonomics: prefer make_shared at call sites.
    auto p = std::make_shared<std::string>(std::string(32, 'x'));
    assert(p->size() == 32);
    assert(p.use_count() == 1);

    // Tradeoff: with make_shared, storage may remain until last weak_ptr dies
    // (object and control block share one allocation).
    std::weak_ptr<std::string> wp = p;
    assert(!wp.expired());
    p.reset();
    assert(wp.expired());
    assert(wp.lock() == nullptr);

    // Both forms manage the same interface after construction.
    auto q = std::make_shared<Counted>(9);
    auto r = q;
    assert(q.use_count() == 2);
    assert(r->value == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/make_shared_single_allocation", run>;

}  // namespace
