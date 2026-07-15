// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : const_ref_extends_temporary
// Topic id : part2/stage05/section05/const_ref_extends_temporary
//
// Covers: const T& extends temporary lifetime; extension does not propagate

#include "learn/topic_registry.hpp"

#include <string>

namespace {

struct Box {
    static int live;
    std::string s;
    explicit Box(std::string v) : s(std::move(v)) { ++live; }
    ~Box() { --live; }
};
int Box::live = 0;

Box make_box() {
    return Box{"payload"};
}

void demo_basics() {
    Box::live = 0;
    {
        const Box& r = make_box();  // temporary lifetime extended to r's scope
        LEARN_CHECK(r.s == "payload");
        LEARN_CHECK(Box::live == 1);
    }
    LEARN_CHECK(Box::live == 0);
}

void demo_intermediate() {
    Box::live = 0;
    {
        const std::string& r = std::string("hello");
        LEARN_CHECK(r == "hello");
        LEARN_CHECK(r.size() == 5);
    }

    // Non-const lvalue ref cannot bind temporary (would not compile):
    // std::string& bad = std::string("x");
    const std::string& ok = std::string("x");
    LEARN_CHECK(ok == "x");
}

void demo_expert() {
    // Extension applies only to the temporary directly bound — not to
    // subobjects of a temporary once you only keep a member reference.
    // Safe pattern: extend the whole object, then use members.
    {
        const Box& box = make_box();
        const std::string& s = box.s;  // s refers into still-alive box
        LEARN_CHECK(s == "payload");
        LEARN_CHECK(Box::live == 1);
    }
    LEARN_CHECK(Box::live == 0);

    // Safe alternative to dangling member bind: copy the value.
    std::string copy = make_box().s;
    LEARN_CHECK(copy == "payload");
    LEARN_CHECK(Box::live == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section05/const_ref_extends_temporary", run>;

}  // namespace
