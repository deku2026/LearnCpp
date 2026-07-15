// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : ebo_no_unique_address
// Topic id : part2/stage15/section04/ebo_no_unique_address
//
// Covers: EBO / [[no_unique_address]]

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <type_traits>

#if defined(_MSC_VER) && !defined(__clang__)
#define LEARN_NUA [[msvc::no_unique_address]]
#elif defined(_MSC_VER) && defined(__clang__)
#define LEARN_NUA [[msvc::no_unique_address]]
#else
#define LEARN_NUA [[no_unique_address]]
#endif

namespace {

struct Empty {};
struct Holder {
    int x;
    Empty e;
};

struct Compact {
    int x;
    LEARN_NUA Empty e;
};

void demo_basics() {
    LEARN_CHECK(sizeof(Empty) >= 1);
    Holder h{1, {}};
    LEARN_CHECK(h.x == 1);
}

void demo_intermediate() {
    Compact c{2, {}};
    LEARN_CHECK(c.x == 2);
    // With no_unique_address, sizeof(Compact) may equal sizeof(int).
    LEARN_CHECK(sizeof(Compact) >= sizeof(int));
}

void demo_expert() {
    static_assert(std::is_empty_v<Empty>);
    Compact c{3, {}};
    LEARN_CHECK(c.x == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/ebo_no_unique_address", run>;

}  // namespace
