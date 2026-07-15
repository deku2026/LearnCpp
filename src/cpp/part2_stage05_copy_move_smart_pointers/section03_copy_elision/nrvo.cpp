// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : nrvo
// Topic id : part2/stage05/section03/nrvo
//
// Covers: NRVO — named return value optimization (allowed, not guaranteed)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

struct Noisy {
    static int ctors;
    static int copies;
    static int moves;

    int value = 0;

    Noisy() { ++ctors; }
    explicit Noisy(int v) : value(v) { ++ctors; }
    Noisy(const Noisy& o) : value(o.value) { ++copies; }
    Noisy(Noisy&& o) noexcept : value(o.value) { ++moves; }
};
int Noisy::ctors = 0;
int Noisy::copies = 0;
int Noisy::moves = 0;

Noisy makeNRVO() {
    Noisy n(99);
    return n;  // NRVO candidate
}

void demo_basics() {
    Noisy::ctors = Noisy::copies = Noisy::moves = 0;
    Noisy b = makeNRVO();
    assert(b.value == 99);
    // Prefer: only construct. Fallback: construct + move. Never need a copy.
    assert(Noisy::ctors >= 1);
    assert(Noisy::copies == 0);
    assert(Noisy::moves <= 1);
}

void demo_intermediate() {
    auto make = []() -> std::string {
        std::string local = "named";
        local += "-ret";
        return local;
    };
    assert(make() == "named-ret");
}

void demo_expert() {
    // Multiple return paths often inhibit NRVO; implicit move still applies.
    auto maybe = [](bool flag) -> Noisy {
        Noisy a(1);
        Noisy b(2);
        if (flag) {
            return a;
        }
        return b;
    };

    Noisy::ctors = Noisy::copies = Noisy::moves = 0;
    Noisy x = maybe(true);
    assert(x.value == 1);
    assert(Noisy::copies == 0);
    // At least construction of both locals; moves may occur when NRVO is off.
    assert(Noisy::ctors >= 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section03/nrvo", run>;

}  // namespace
