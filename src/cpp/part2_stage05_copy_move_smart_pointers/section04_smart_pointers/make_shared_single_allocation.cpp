// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : make_shared_single_allocation
// Topic id : part2/stage05/section04/make_shared_single_allocation
//
// Covers: make_shared usually one allocation (object + control block together)

#include "learn/topic_registry.hpp"

#include <memory>
#include <string>

namespace {

struct Counted {
    int value;
    explicit Counted(int v) : value(v) {}
};

void demo_basics() {
    auto p = std::make_shared<int>(42);
    LEARN_CHECK(p.use_count() == 1);
    LEARN_CHECK(*p == 42);
}

void demo_intermediate() {
    // make_shared: typically one heap allocation (object + control block together).
    // shared_ptr(new T): two allocations (object, then control block).
    auto a = std::make_shared<Counted>(1);
    auto b = std::shared_ptr<Counted>(new Counted(2));
    LEARN_CHECK(a->value == 1);
    LEARN_CHECK(b->value == 2);
    LEARN_CHECK(a.use_count() == 1);
    LEARN_CHECK(b.use_count() == 1);
    LEARN_CHECK(a.get() != b.get());
}

void demo_expert() {
    // Exception-safety / ergonomics: prefer make_shared at call sites.
    auto p = std::make_shared<std::string>(std::string(32, 'x'));
    LEARN_CHECK(p->size() == 32);
    LEARN_CHECK(p.use_count() == 1);

    // Tradeoff: with make_shared, storage may remain until last weak_ptr dies
    // (object and control block share one allocation).
    std::weak_ptr<std::string> wp = p;
    LEARN_CHECK(!wp.expired());
    p.reset();
    LEARN_CHECK(wp.expired());
    LEARN_CHECK(wp.lock() == nullptr);

    // Both forms manage the same interface after construction.
    auto q = std::make_shared<Counted>(9);
    auto r = q;
    LEARN_CHECK(q.use_count() == 2);
    LEARN_CHECK(r->value == 9);
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
