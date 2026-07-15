// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : weak_ptr_break_cycles
// Topic id : part2/stage05/section04/weak_ptr_break_cycles
//
// Covers: weak_ptr breaks shared_ptr cycles; lock() for safe access

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>

namespace {

struct Node {
    static int live;
    std::shared_ptr<Node> next;  // owning edge
    std::weak_ptr<Node> prev;    // observing edge (breaks cycles)
    explicit Node() { ++live; }
    ~Node() { --live; }
};
int Node::live = 0;

void demo_basics() {
    Node::live = 0;
    {
        auto a = std::make_shared<Node>();
        auto b = std::make_shared<Node>();
        a->next = b;
        b->prev = a;  // weak: does not bump a's strong count
        assert(a.use_count() == 1);
        assert(b.use_count() == 2);  // a->next + b
        assert(Node::live == 2);
    }
    assert(Node::live == 0);  // no leak
}

void demo_intermediate() {
    Node::live = 0;
    auto a = std::make_shared<Node>();
    auto b = std::make_shared<Node>();
    a->next = b;
    b->prev = a;

    if (auto sp = b->prev.lock()) {
        assert(sp.get() == a.get());
        assert(sp.use_count() >= 2);
    } else {
        assert(false);
    }

    a.reset();
    // a destroyed: b->prev expired, b still held by local
    assert(b->prev.expired());
    assert(b.use_count() == 1);
    b.reset();
    assert(Node::live == 0);
}

void demo_expert() {
    // Contrast: mutual shared_ptr would leak (not executed as a leak).
    // We only show the correct weak pattern under ASan-friendly asserts.
    Node::live = 0;
    {
        auto head = std::make_shared<Node>();
        auto tail = std::make_shared<Node>();
        head->next = tail;
        tail->prev = head;
        auto mid = std::make_shared<Node>();
        tail->next = mid;
        mid->prev = tail;
        assert(Node::live == 3);
    }
    assert(Node::live == 0);

    std::weak_ptr<int> w;
    {
        auto p = std::make_shared<int>(5);
        w = p;
        assert(!w.expired());
    }
    assert(w.expired());
    assert(w.lock() == nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/weak_ptr_break_cycles", run>;

}  // namespace
