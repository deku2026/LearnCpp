// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : shared_ptr_shared
// Topic id : part2/stage05/section04/shared_ptr_shared
//
// Covers: shared_ptr shared ownership, use_count, last owner deletes

#include "learn/topic_registry.hpp"

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
    LEARN_CHECK(a.use_count() == 1);
    LEARN_CHECK(Widget::live == 1);
    {
        auto b = a;
        LEARN_CHECK(a.use_count() == 2);
        LEARN_CHECK(b.use_count() == 2);
        LEARN_CHECK(b->id == 1);
    }
    LEARN_CHECK(a.use_count() == 1);
    LEARN_CHECK(Widget::live == 1);
    a.reset();
    LEARN_CHECK(Widget::live == 0);
}

void demo_intermediate() {
    Widget::live = 0;
    auto a = std::make_shared<Widget>(2);
    auto b = a;
    auto c = std::move(b);
    LEARN_CHECK(b == nullptr);
    LEARN_CHECK(a.use_count() == 2);
    LEARN_CHECK(c.use_count() == 2);
    LEARN_CHECK(c->id == 2);
}

void demo_expert() {
    Widget::live = 0;
    std::shared_ptr<Widget> empty;
    LEARN_CHECK(empty.use_count() == 0);
    LEARN_CHECK(!empty);

    auto p = std::make_shared<Widget>(3);
    empty = p;
    LEARN_CHECK(empty.use_count() == 2);
    p.reset();
    LEARN_CHECK(empty.use_count() == 1);
    empty.reset();
    LEARN_CHECK(Widget::live == 0);

    // Alias constructor: share ownership of parent, point at member.
    struct Pair {
        int x = 10;
        int y = 20;
    };
    auto pair = std::make_shared<Pair>();
    std::shared_ptr<int> y_view(pair, &pair->y);
    LEARN_CHECK(*y_view == 20);
    LEARN_CHECK(y_view.use_count() == pair.use_count());
    pair.reset();
    LEARN_CHECK(*y_view == 20);  // still alive via aliasing shared_ptr
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
