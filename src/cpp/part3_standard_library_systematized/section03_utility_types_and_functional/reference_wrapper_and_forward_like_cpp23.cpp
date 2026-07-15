// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : reference_wrapper_and_forward_like_cpp23
// Topic id : part3/section03/reference_wrapper_and_forward_like_cpp23
//
// Covers: std::reference_wrapper and forward_like C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <utility>
#include <vector>
#include <version>

namespace {

void demo_basics() {
    int x = 10;
    std::reference_wrapper<int> r = x;
    r.get() = 20;
    assert(x == 20);
}

void demo_intermediate() {
    int a = 1;
    int b = 2;
    std::vector<std::reference_wrapper<int>> v{a, b};
    v[0].get() = 9;
    assert(a == 9);
}

void demo_expert() {
#if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
    int x = 1;
    auto&& y = std::forward_like<int&>(x);
    y = 5;
    assert(x == 5);
#else
    int x = 1;
    int& y = x;
    y = 5;
    assert(x == 5);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/reference_wrapper_and_forward_like_cpp23", run>;

}  // namespace
