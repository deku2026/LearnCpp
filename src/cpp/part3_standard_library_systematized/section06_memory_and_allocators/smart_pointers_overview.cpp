// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : smart_pointers_overview
// Topic id : part3/section06/smart_pointers_overview
//
// Covers: unique_ptr shared_ptr weak_ptr ownership

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>

namespace {

void demo_basics() {
    auto u = std::make_unique<int>(5);
    assert(*u == 5);
    auto s = std::make_shared<int>(9);
    assert(*s == 9);
    assert(s.use_count() == 1);
}

void demo_intermediate() {
    auto s = std::make_shared<int>(1);
    std::weak_ptr<int> w = s;
    assert(!w.expired());
    {
        auto locked = w.lock();
        assert(locked);
        assert(*locked == 1);
    }
    s.reset();
    assert(w.expired());
}

void demo_expert() {
    std::unique_ptr<int[]> arr = std::make_unique<int[]>(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    assert(arr[2] == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/smart_pointers_overview", run>;

}  // namespace
