// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : enable_shared_from_this
// Topic id : part2/stage05/section04/enable_shared_from_this
//
// Covers: shared_from_this shares existing control block; never shared_ptr(this)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <stdexcept>

namespace {

struct Widget : std::enable_shared_from_this<Widget> {
    int id = 0;
    explicit Widget(int i) : id(i) {}

    std::shared_ptr<Widget> getSelf() { return shared_from_this(); }
};

void demo_basics() {
    auto w = std::make_shared<Widget>(1);
    auto self = w->getSelf();
    assert(self.get() == w.get());
    assert(w.use_count() == 2);
    assert(self->id == 1);
}

void demo_intermediate() {
    auto w = std::make_shared<Widget>(2);
    {
        auto self = w->shared_from_this();
        assert(w.use_count() == 2);
    }
    assert(w.use_count() == 1);
}

void demo_expert() {
    // shared_from_this requires an existing shared_ptr owner.
    bool threw = false;
    try {
        Widget stack(3);  // not managed by shared_ptr
        (void)stack.shared_from_this();
    } catch (const std::bad_weak_ptr&) {
        threw = true;
    }
    assert(threw);

    // Correct pattern: construct via make_shared first.
    auto w = std::make_shared<Widget>(4);
    std::weak_ptr<Widget> weak = w->weak_from_this();
    assert(!weak.expired());
    w.reset();
    assert(weak.expired());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/enable_shared_from_this", run>;

}  // namespace
