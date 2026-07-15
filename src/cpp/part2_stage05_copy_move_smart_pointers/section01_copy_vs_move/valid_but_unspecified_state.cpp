// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : valid_but_unspecified_state
// Topic id : part2/stage05/section01/valid_but_unspecified_state
//
// Covers: moved-from objects: only precondition-free ops are safe

#include "learn/topic_registry.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

void demo_basics() {
    std::string str = "example";
    std::string sink = std::move(str);
    LEARN_CHECK(sink == "example");

    // Safe on moved-from: empty/clear/assign/destroy
    (void)str.empty();
    str.clear();
    str = "new value";
    LEARN_CHECK(str == "new value");
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4};
    std::vector<int> w = std::move(v);
    LEARN_CHECK(w.size() == 4);

    // Valid but unspecified: do not assume size/capacity.
    v.clear();
    LEARN_CHECK(v.empty());
    v.push_back(99);
    LEARN_CHECK(v.size() == 1);
    LEARN_CHECK(v[0] == 99);
}

void demo_expert() {
    // unique_ptr moved-from state is fully specified: nullptr
    auto p = std::make_unique<int>(42);
    auto q = std::move(p);
    LEARN_CHECK(q != nullptr);
    LEARN_CHECK(*q == 42);
    LEARN_CHECK(p == nullptr);

    // Guard before use after potential move.
    if (p) {
        LEARN_CHECK(false);  // should not run
    } else {
        p = std::make_unique<int>(7);
        LEARN_CHECK(*p == 7);
    }

    std::string a = "x";
    std::string b = std::move(a);
    if (!a.empty()) {
        (void)a.back();  // only if non-empty
    }
    a.assign("rebuilt");
    LEARN_CHECK(a == "rebuilt");
    LEARN_CHECK(b == "x");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/valid_but_unspecified_state", run>;

}  // namespace
