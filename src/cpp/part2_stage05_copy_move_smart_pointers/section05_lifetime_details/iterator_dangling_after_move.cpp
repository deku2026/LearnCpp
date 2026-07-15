// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : iterator_dangling_after_move
// Topic id : part2/stage05/section05/iterator_dangling_after_move
//
// Covers: iterators/refs may dangle after move or reallocation — safe usage

#include "learn/topic_registry.hpp"

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
    LEARN_CHECK(b.size() == 3);
    LEARN_CHECK(b[0] == 1);
    a.clear();  // safe precondition-free op on moved-from vector
    a.push_back(9);
    LEARN_CHECK(a.size() == 1);
}

void demo_intermediate() {
    std::vector<std::string> v{"x", "y"};
    auto it = v.begin();
    LEARN_CHECK(*it == "x");

    // Reallocation invalidates iterators — take values or re-get after growth.
    const std::string first = *it;
    v.push_back("z");
    v.push_back("w");
    LEARN_CHECK(first == "x");
    LEARN_CHECK(v.front() == "x");

    // After move of element, original element is valid-but-unspecified.
    std::string sink = std::move(v[0]);
    LEARN_CHECK(sink == "x");
    v[0] = "X";  // reassign is safe
    LEARN_CHECK(v[0] == "X");
}

void demo_expert() {
    // unique_ptr move leaves source null — check before use.
    auto p = std::make_unique<int>(42);
    auto q = std::move(p);
    LEARN_CHECK(p == nullptr);
    LEARN_CHECK(q != nullptr && *q == 42);

    // vector of unique_ptr: move container, then iterate the destination.
    std::vector<std::unique_ptr<int>> src;
    src.push_back(std::make_unique<int>(1));
    src.push_back(std::make_unique<int>(2));
    std::vector<std::unique_ptr<int>> dst = std::move(src);
    LEARN_CHECK(dst.size() == 2);
    LEARN_CHECK(*dst[0] == 1);
    LEARN_CHECK(*dst[1] == 2);
    // src iterators from before the move must not be used; rebuild if needed.
    src.clear();
    LEARN_CHECK(src.empty());

    // Safe pattern: never store raw pointers into container buffers across moves.
    std::vector<int> buf{10, 20, 30};
    const int value = buf[1];  // copy
    std::vector<int> other = std::move(buf);
    LEARN_CHECK(value == 20);
    LEARN_CHECK(other[1] == 20);
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
