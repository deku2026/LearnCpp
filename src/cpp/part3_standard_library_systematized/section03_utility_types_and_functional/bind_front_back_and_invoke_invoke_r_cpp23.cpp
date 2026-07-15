// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : bind_front_back_and_invoke_invoke_r_cpp23
// Topic id : part3/section03/bind_front_back_and_invoke_invoke_r_cpp23
//
// Covers: std::bind_front/back, invoke, invoke_r

#include "learn/topic_registry.hpp"

#include <functional>
#include <version>

namespace {

void demo_basics() {
    auto add = [](int a, int b) { return a + b; };
    LEARN_CHECK(std::invoke(add, 2, 3) == 5);
}

void demo_intermediate() {
    auto add = [](int a, int b, int c) { return a + b + c; };
    auto plus1 = std::bind_front(add, 1);
    LEARN_CHECK(plus1(2, 3) == 6);
#if defined(__cpp_lib_bind_back) && __cpp_lib_bind_back >= 202202L
    auto tail3 = std::bind_back(add, 3);
    LEARN_CHECK(tail3(1, 2) == 6);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_invoke_r) && __cpp_lib_invoke_r >= 202106L
    auto f = [](int x) { return x * 2; };
    long r = std::invoke_r<long>(f, 4);
    LEARN_CHECK(r == 8);
#else
    auto f = [](int x) { return x * 2; };
    LEARN_CHECK(std::invoke(f, 4) == 8);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/bind_front_back_and_invoke_invoke_r_cpp23", run>;

}  // namespace
