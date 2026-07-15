// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : crtp_idiom
// Topic id : part2/stage15/section02/crtp_idiom
//
// Covers: CRTP static polymorphism

#include "learn/topic_registry.hpp"

#include <string>

namespace {

template <class D>
struct Counter {
    int ticks = 0;
    void hit() {
        ++ticks;
        static_cast<D*>(this)->on_hit();
    }
};

struct Logger : Counter<Logger> {
    int last = 0;
    void on_hit() { last = ticks; }
};

void demo_basics() {
    Logger l;
    l.hit();
    LEARN_CHECK(l.ticks == 1);
    LEARN_CHECK(l.last == 1);
}

void demo_intermediate() {
    Logger l;
    l.hit();
    l.hit();
    LEARN_CHECK(l.ticks == 2);
}

void demo_expert() {
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    struct Modern {
        int n = 0;
        int get(this const Modern& self) { return self.n; }
        void set(this Modern& self, int v) { self.n = v; }
    };
    Modern m;
    m.set(3);
    LEARN_CHECK(m.get() == 3);
#else
    // Classic CRTP still works without deducing this.
    Logger l;
    l.hit();
    LEARN_CHECK(l.last == 1);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section02/crtp_idiom", run>;

}  // namespace
