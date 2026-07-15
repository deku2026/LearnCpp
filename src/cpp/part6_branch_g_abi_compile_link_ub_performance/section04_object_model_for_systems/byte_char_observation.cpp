// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : byte_char_observation
// Topic id : part6/g/section04/byte_char_observation
//
// Covers: std::byte / unsigned char may inspect object representation

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstring>

namespace {

void demo_basics() {
    int x = 0x01020304;
    auto* bytes = reinterpret_cast<const std::byte*>(&x);
    std::byte acc{0};
    for (std::size_t i = 0; i < sizeof(int); ++i) acc = acc | bytes[i];
    LEARN_CHECK(acc != std::byte{0} || x == 0);
}

void demo_intermediate() {
    int x = 7;
    unsigned char buf[sizeof(int)];
    std::memcpy(buf, &x, sizeof(int));
    int y = 0;
    std::memcpy(&y, buf, sizeof(int));
    LEARN_CHECK(y == 7);
}

void demo_expert() {
    std::byte b{0xFF};
    LEARN_CHECK(std::to_integer<int>(b) == 255);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/byte_char_observation", run>;

}  // namespace
