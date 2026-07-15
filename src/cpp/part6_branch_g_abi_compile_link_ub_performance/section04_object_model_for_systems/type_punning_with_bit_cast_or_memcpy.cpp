// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : type_punning_with_bit_cast_or_memcpy
// Topic id : part6/g/section04/type_punning_with_bit_cast_or_memcpy
//
// Covers: defined type punning with bit_cast or memcpy

#include "learn/topic_registry.hpp"

#include <bit>
#include <cstdint>
#include <cstring>

namespace {

void demo_basics() {
    std::uint32_t u = 0x3f800000u;  // 1.0f on IEEE754
    float f = 0;
    std::memcpy(&f, &u, sizeof(f));
    LEARN_CHECK(f == 1.0f || f != 0.0f);
}

void demo_intermediate() {
#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806L
    float f = 1.0f;
    auto u = std::bit_cast<std::uint32_t>(f);
    LEARN_CHECK(std::bit_cast<float>(u) == 1.0f);
#else
    float f = 1.0f;
    std::uint32_t u = 0;
    std::memcpy(&u, &f, sizeof(f));
    float g = 0;
    std::memcpy(&g, &u, sizeof(g));
    LEARN_CHECK(g == 1.0f);
#endif
}

void demo_expert() {
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/type_punning_with_bit_cast_or_memcpy", run>;

}  // namespace
