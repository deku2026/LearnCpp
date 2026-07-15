// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : strict_aliasing_violation_deep
// Topic id : part6/g/section03/strict_aliasing_violation_deep
//
// Covers: strict aliasing: type-pun via memcpy/bit_cast not via wrong-type lvalue

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>

namespace {

void demo_basics() {
    float f = 1.0f;
    std::uint32_t bits = 0;
    std::memcpy(&bits, &f, sizeof(bits));
    assert(bits != 0);
}

void demo_intermediate() {
#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806L
    float f = 2.0f;
    auto bits = std::bit_cast<std::uint32_t>(f);
    auto back = std::bit_cast<float>(bits);
    assert(back == f);
#else
    float f = 2.0f;
    std::uint32_t bits = 0;
    std::memcpy(&bits, &f, sizeof(bits));
    float back = 0;
    std::memcpy(&back, &bits, sizeof(back));
    assert(back == f);
#endif
}

void demo_expert() {
    // Not executed: int* ip = (int*)&f; *ip; // classic strict-aliasing violation
    int x = 7;
    int* p = &x;
    assert(*p == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/strict_aliasing_violation_deep", run>;

}  // namespace
