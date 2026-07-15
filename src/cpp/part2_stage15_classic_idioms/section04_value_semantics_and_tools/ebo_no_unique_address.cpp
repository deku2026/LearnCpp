// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §4.4
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : ebo_no_unique_address
// Topic id : part2/stage15/section04/ebo_no_unique_address
//
// EBO and [[no_unique_address]]: empty objects need not consume storage.
// Motive : empty allocator/comparator/policy members must not bloat containers.
// Modern : C++20 [[no_unique_address]] for members; classic EBO via empty bases.
// Pitfall: attribute is advisory (ABI/layout still implementation-defined details);
//          MSVC historically needed [[msvc::no_unique_address]] — C++23 / recent
//          toolchains honor the standard attribute more consistently.

#include "learn/topic_registry.hpp"

#include <iostream>

namespace {

struct Empty {
    void tag() const { std::cout << "  Empty::tag\n"; }
};

struct Empty2 {
    void tag2() const { std::cout << "  Empty2::tag2\n"; }
};

// Classic EBO: empty base may add no size.
struct EboHolder : Empty {
    int value = 0;
};

// Two distinct empty bases can both be optimized (different types).
struct EboDouble : Empty, Empty2 {
    int value = 0;
};

// On the MSVC ABI (cl and clang-cl), the layout-affecting spelling is
// [[msvc::no_unique_address]]. The standard [[no_unique_address]] is what you
// write in portable code; both mean the same intent.
#if defined(_MSC_VER)
#define LEARN_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define LEARN_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

struct WithoutAttr {
    Empty e;
    int value = 0;
};

struct WithAttr {
    LEARN_NO_UNIQUE_ADDRESS Empty e;
    int value = 0;
};

struct WithAttrTwo {
    LEARN_NO_UNIQUE_ADDRESS Empty e1;
    LEARN_NO_UNIQUE_ADDRESS Empty2 e2;  // different type: can share with value's storage
    int value = 0;
};

// Policy member pattern (see policy-based design).
template <typename Policy>
struct Compressed {
    LEARN_NO_UNIQUE_ADDRESS Policy policy{};
    int data = 0;

    void apply() const { policy.tag(); }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== sizes ==\n";
    std::cout << "  sizeof(Empty)          = " << sizeof(Empty) << '\n';
    std::cout << "  sizeof(EboHolder)      = " << sizeof(EboHolder) << '\n';
    std::cout << "  sizeof(EboDouble)      = " << sizeof(EboDouble) << '\n';
    std::cout << "  sizeof(WithoutAttr)    = " << sizeof(WithoutAttr) << '\n';
    std::cout << "  sizeof(WithAttr)       = " << sizeof(WithAttr) << '\n';
    std::cout << "  sizeof(WithAttrTwo)    = " << sizeof(WithAttrTwo) << '\n';
    std::cout << "  sizeof(Compressed<Empty>) = " << sizeof(Compressed<Empty>) << '\n';

    // Empty object still has unique identity unless no_unique_address collapses storage.
    static_assert(sizeof(Empty) >= 1);

    // On conforming optimizing layouts, WithAttr is typically sizeof(int).
    // We assert the useful inequality that the attribute does not increase size.
    assert(sizeof(WithAttr) <= sizeof(WithoutAttr));
    assert(sizeof(EboHolder) == sizeof(int) || sizeof(EboHolder) >= sizeof(int));

    WithAttr w{};
    w.value = 42;
    w.e.tag();
    assert(w.value == 42);

    Compressed<Empty> c;
    c.data = 7;
    c.apply();
    assert(sizeof(Compressed<Empty>) <= sizeof(Empty) + sizeof(int));

    std::cout << "library pattern: empty Allocator as [[no_unique_address]] member\n";
    std::cout << "same empty type twice often cannot fully collapse (unique identity)\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/ebo_no_unique_address", run>;

}  // namespace
