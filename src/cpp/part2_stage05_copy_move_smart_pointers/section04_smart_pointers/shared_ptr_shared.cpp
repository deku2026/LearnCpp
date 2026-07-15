// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : shared_ptr_shared
// Topic id : part2/stage05/section04/shared_ptr_shared
//
// Covers: shared_ptr shared ownership, use_count, last owner deletes

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>

namespace {

struct Widget {
    static int live;
    int id;
    explicit Widget(int i) : id(i) { ++live; }
    ~Widget() { --live; }
};
int Widget::live = 0;

void demo_basics() {
    Widget::live = 0;
    auto a = std::make_shared<Widget>(1);
    assert(a.use_count() == 1);
    assert(Widget::live == 1);
    {
        auto b = a;
        assert(a.use_count() == 2);
        assert(b.use_count() == 2);
        assert(b->id == 1);
    }
    assert(a.use_count() == 1);
    assert(Widget::live == 1);
    a.reset();
    assert(Widget::live == 0);
}

void demo_intermediate() {
    Widget::live = 0;
    auto a = std::make_shared<Widget>(2);
    auto b = a;
    auto c = std::move(b);
    assert(b == nullptr);
    assert(a.use_count() == 2);
    assert(c.use_count() == 2);
    assert(c->id == 2);
}

void demo_expert() {
    Widget::live = 0;
    std::shared_ptr<Widget> empty;
    assert(empty.use_count() == 0);
    assert(!empty);

    auto p = std::make_shared<Widget>(3);
    empty = p;
    assert(empty.use_count() == 2);
    p.reset();
    assert(empty.use_count() == 1);
    empty.reset();
    assert(Widget::live == 0);

    // Alias constructor: share ownership of parent, point at member.
    struct Pair {
        int x = 10;
        int y = 20;
    };
    auto pair = std::make_shared<Pair>();
    std::shared_ptr<int> y_view(pair, &pair->y);
    assert(*y_view == 20);
    assert(y_view.use_count() == pair.use_count());
    pair.reset();
    assert(*y_view == 20);  // still alive via aliasing shared_ptr
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/shared_ptr_shared", run>;

}  // namespace
