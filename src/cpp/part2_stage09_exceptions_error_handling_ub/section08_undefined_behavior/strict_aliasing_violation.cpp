// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : strict_aliasing_violation
// Topic id : part2/stage09/section08/strict_aliasing_violation
//
// Covers: strict aliasing — use memcpy / bit_cast, not illegal type punning

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <version>

namespace {

std::uint32_t float_bits(float f) {
#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806L
    return std::bit_cast<std::uint32_t>(f);
#else
    std::uint32_t u = 0;
    static_assert(sizeof(float) == sizeof(std::uint32_t));
    std::memcpy(&u, &f, sizeof(u));
    return u;
#endif
}

float bits_to_float(std::uint32_t u) {
#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806L
    return std::bit_cast<float>(u);
#else
    float f = 0;
    std::memcpy(&f, &u, sizeof(f));
    return f;
#endif
}

void demo_basics() {
    float f = 1.0f;
    auto bits = float_bits(f);
    assert(bits_to_float(bits) == 1.0f);
}

void demo_intermediate() {
    // char* may alias; this is allowed for byte inspection.
    int x = 0x01020304;
    const auto* p = reinterpret_cast<const unsigned char*>(&x);
    assert(p[0] == 0x04 || p[0] == 0x01);  // endian-dependent
}

void demo_expert() {
    // Comment only: `float f = 1; int i = *(int*)&f;` is classic strict-aliasing UB.
    // SAFE: memcpy/bit_cast as above.
    double d = 2.5;
    std::uint64_t u = 0;
    static_assert(sizeof(d) == sizeof(u));
    std::memcpy(&u, &d, sizeof(u));
    double back = 0;
    std::memcpy(&back, &u, sizeof(back));
    assert(back == 2.5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/strict_aliasing_violation", run>;

}  // namespace
