// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : object_representation
// Topic id : part6/a/section01/object_representation
//
// Covers: object representation bytes, value representation, memcpy of trivially copyable

#include "learn/topic_registry.hpp"

#include <cstring>
#include <type_traits>

namespace {

struct Point {
    int x;
    int y;
};

void demo_basics() {
    static_assert(std::is_trivially_copyable_v<Point>);
    Point a{1, 2};
    Point b{};
    std::memcpy(&b, &a, sizeof(Point));
    LEARN_CHECK(b.x == 1 && b.y == 2);
}

void demo_intermediate() {
    Point p{0x01020304, 0};
    const auto* bytes = reinterpret_cast<const unsigned char*>(&p);
    // Object representation is sizeof(Point) bytes; first sizeof(int) are x's bytes.
    unsigned char sum = 0;
    for (std::size_t i = 0; i < sizeof(int); ++i) {
        sum = static_cast<unsigned char>(sum + bytes[i]);
    }
    LEARN_CHECK(sum != 0 || p.x == 0);
    LEARN_CHECK(sizeof(Point) == 2 * sizeof(int));
}

void demo_expert() {
    // Value representation may ignore padding bits; only trivially copyable
    // types may be memcpy'd as whole objects safely.
    struct WithPad {
        char c;
        int i;
    };
    static_assert(std::is_trivially_copyable_v<WithPad>);
    WithPad a{'Z', 7};
    WithPad b{};
    std::memcpy(&b, &a, sizeof(WithPad));
    LEARN_CHECK(b.c == 'Z' && b.i == 7);
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section01/object_representation", run>;

}  // namespace
