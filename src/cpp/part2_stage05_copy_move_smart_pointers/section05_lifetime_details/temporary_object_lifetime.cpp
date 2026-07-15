// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : temporary_object_lifetime
// Topic id : part2/stage05/section05/temporary_object_lifetime
//
// Covers: temporary lifetime ends at full-expression unless extended

#include "learn/topic_registry.hpp"

#include <string>
#include <utility>

namespace {

struct Tracker {
    static int live;
    int id;
    explicit Tracker(int i) : id(i) { ++live; }
    Tracker(const Tracker& o) : id(o.id) { ++live; }
    Tracker(Tracker&& o) noexcept : id(o.id) { ++live; }
    ~Tracker() { --live; }
};
int Tracker::live = 0;

Tracker make_tracker(int id) {
    return Tracker{id};
}

void demo_basics() {
    Tracker::live = 0;
    {
        Tracker t = make_tracker(1);  // temporary materialized / elided into t
        LEARN_CHECK(t.id == 1);
        LEARN_CHECK(Tracker::live == 1);
    }
    LEARN_CHECK(Tracker::live == 0);
}

void demo_intermediate() {
    Tracker::live = 0;
    // Full-expression lifetime: temporary dies at semicolon.
    LEARN_CHECK(make_tracker(2).id == 2);
    LEARN_CHECK(Tracker::live == 0);

    // Binding const ref extends temporary (see also const_ref_extends_temporary).
    {
        const Tracker& r = make_tracker(3);
        LEARN_CHECK(r.id == 3);
        LEARN_CHECK(Tracker::live == 1);
    }
    LEARN_CHECK(Tracker::live == 0);
}

void demo_expert() {
    // Temporary as function argument lives until the full call expression ends.
    auto use = [](const Tracker& t) { return t.id; };
    LEARN_CHECK(use(make_tracker(4)) == 4);
    LEARN_CHECK(Tracker::live == 0);

    // prvalue string temporary used immediately is safe.
    LEARN_CHECK(std::string("tmp").size() == 3);

    // Storing a pointer/reference into a dying temporary is the classic trap;
    // we only show the safe pattern: copy the value out.
    int id = make_tracker(5).id;
    LEARN_CHECK(id == 5);
    LEARN_CHECK(Tracker::live == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section05/temporary_object_lifetime", run>;

}  // namespace
