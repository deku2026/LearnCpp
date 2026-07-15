// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : iterator_dangling_after_move
// Topic id : part2/stage05/section05/iterator_dangling_after_move
//
// Covers: iterators/refs may dangle after move or reallocation — safe usage

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> a{1, 2, 3};
    std::vector<int> b = std::move(a);
    // After move, do not use iterators/refs obtained from a.
    // Re-acquire from the new owner.
    assert(b.size() == 3);
    assert(b[0] == 1);
    a.clear();  // safe precondition-free op on moved-from vector
    a.push_back(9);
    assert(a.size() == 1);
}

void demo_intermediate() {
    std::vector<std::string> v{"x", "y"};
    auto it = v.begin();
    assert(*it == "x");

    // Reallocation invalidates iterators — take values or re-get after growth.
    const std::string first = *it;
    v.push_back("z");
    v.push_back("w");
    assert(first == "x");
    assert(v.front() == "x");

    // After move of element, original element is valid-but-unspecified.
    std::string sink = std::move(v[0]);
    assert(sink == "x");
    v[0] = "X";  // reassign is safe
    assert(v[0] == "X");
}

void demo_expert() {
    // unique_ptr move leaves source null — check before use.
    auto p = std::make_unique<int>(42);
    auto q = std::move(p);
    assert(p == nullptr);
    assert(q != nullptr && *q == 42);

    // vector of unique_ptr: move container, then iterate the destination.
    std::vector<std::unique_ptr<int>> src;
    src.push_back(std::make_unique<int>(1));
    src.push_back(std::make_unique<int>(2));
    std::vector<std::unique_ptr<int>> dst = std::move(src);
    assert(dst.size() == 2);
    assert(*dst[0] == 1);
    assert(*dst[1] == 2);
    // src iterators from before the move must not be used; rebuild if needed.
    src.clear();
    assert(src.empty());

    // Safe pattern: never store raw pointers into container buffers across moves.
    std::vector<int> buf{10, 20, 30};
    const int value = buf[1];  // copy
    std::vector<int> other = std::move(buf);
    assert(value == 20);
    assert(other[1] == 20);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section05/iterator_dangling_after_move", run>;

}  // namespace
