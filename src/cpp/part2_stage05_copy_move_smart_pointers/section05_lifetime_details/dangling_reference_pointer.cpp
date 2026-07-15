// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : dangling_reference_pointer
// Topic id : part2/stage05/section05/dangling_reference_pointer
//
// Covers: safe patterns that avoid dangling refs/pointers (no UAF executed)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace {

// BAD (not called): return ref/pointer to local.
// const std::string& bad_ref() { std::string local = "oops"; return local; }

std::string good_by_value() {
    std::string local = "ok";
    return local;  // RVO / move — safe
}

const std::string& good_ref_to_static() {
    static const std::string s = "static";
    return s;  // lifetime is program duration
}

void demo_basics() {
    assert(good_by_value() == "ok");
    assert(good_ref_to_static() == "static");
}

void demo_intermediate() {
    // Container reallocation can invalidate references — avoid holding them across growth.
    std::vector<int> v{1, 2, 3};
    const int first = v[0];  // copy value, not a long-lived reference
    v.push_back(4);
    v.push_back(5);
    assert(first == 1);
    assert(v[0] == 1);

    // After potential reallocation, re-acquire references.
    int& ref = v[0];
    ref = 10;
    assert(v[0] == 10);
}

void demo_expert() {
    // Heap: unique_ptr prevents use-after-free if you don't keep raw owners.
    int* raw_observer = nullptr;
    {
        auto p = std::make_unique<int>(5);
        raw_observer = p.get();  // borrow only while p lives
        assert(*raw_observer == 5);
    }
    // raw_observer is dangling here — do not dereference.
    raw_observer = nullptr;  // sanitize after owner death

    // Prefer returning smart pointers or values over raw owning pointers.
    auto factory = []() { return std::make_unique<std::string>("heap"); };
    auto s = factory();
    assert(*s == "heap");

    // Reference into a still-alive object is fine.
    std::string owner = "alive";
    const std::string& r = owner;
    assert(r == "alive");
    owner += "!";
    assert(r == "alive!");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section05/dangling_reference_pointer", run>;

}  // namespace
