// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : valid_but_unspecified_state
// Topic id : part2/stage05/section01/valid_but_unspecified_state
//
// Covers: moved-from objects: only precondition-free ops are safe

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

void demo_basics() {
    std::string str = "example";
    std::string sink = std::move(str);
    assert(sink == "example");

    // Safe on moved-from: empty/clear/assign/destroy
    (void)str.empty();
    str.clear();
    str = "new value";
    assert(str == "new value");
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4};
    std::vector<int> w = std::move(v);
    assert(w.size() == 4);

    // Valid but unspecified: do not assume size/capacity.
    v.clear();
    assert(v.empty());
    v.push_back(99);
    assert(v.size() == 1);
    assert(v[0] == 99);
}

void demo_expert() {
    // unique_ptr moved-from state is fully specified: nullptr
    auto p = std::make_unique<int>(42);
    auto q = std::move(p);
    assert(q != nullptr);
    assert(*q == 42);
    assert(p == nullptr);

    // Guard before use after potential move.
    if (p) {
        assert(false);  // should not run
    } else {
        p = std::make_unique<int>(7);
        assert(*p == 7);
    }

    std::string a = "x";
    std::string b = std::move(a);
    if (!a.empty()) {
        (void)a.back();  // only if non-empty
    }
    a.assign("rebuilt");
    assert(a == "rebuilt");
    assert(b == "x");
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
