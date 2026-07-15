// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : unique_ptr_exclusive
// Topic id : part2/stage05/section04/unique_ptr_exclusive
//
// Covers: unique_ptr exclusive ownership, move-only, auto delete

#include "learn/topic_registry.hpp"

#include <memory>
#include <utility>
#include <vector>

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
    {
        auto w = std::make_unique<Widget>(1);
        LEARN_CHECK(w != nullptr);
        LEARN_CHECK(w->id == 1);
        LEARN_CHECK(Widget::live == 1);
    }
    LEARN_CHECK(Widget::live == 0);
}

void demo_intermediate() {
    Widget::live = 0;
    auto a = std::make_unique<Widget>(10);
    // auto b = a; // not copyable
    auto b = std::move(a);
    LEARN_CHECK(a == nullptr);
    LEARN_CHECK(b != nullptr);
    LEARN_CHECK(b->id == 10);
    LEARN_CHECK(Widget::live == 1);
}

void demo_expert() {
    Widget::live = 0;
    std::vector<std::unique_ptr<Widget>> vec;
    vec.push_back(std::make_unique<Widget>(1));
    auto p = std::make_unique<Widget>(2);
    vec.push_back(std::move(p));
    LEARN_CHECK(p == nullptr);
    LEARN_CHECK(vec.size() == 2);
    LEARN_CHECK(Widget::live == 2);
    vec.clear();
    LEARN_CHECK(Widget::live == 0);

    // Polymorphic unique_ptr needs virtual destructor on base (shown safely).
    struct Base {
        virtual ~Base() = default;
        virtual int tag() const { return 1; }
    };
    struct Derived : Base {
        int tag() const override { return 2; }
    };
    std::unique_ptr<Base> pb = std::make_unique<Derived>();
    LEARN_CHECK(pb->tag() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/unique_ptr_exclusive", run>;

}  // namespace
