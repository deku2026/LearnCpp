// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : storage_duration_phases
// Topic id : part6/b/section01/storage_duration_phases
//
// Covers: automatic, static, thread, dynamic storage duration phases

#include "learn/topic_registry.hpp"

#include <thread>

namespace {

int g_static = 0;

void demo_basics() {
    int automatic = 1;  // automatic storage
    static int local_static = 0;
    ++local_static;
    LEARN_CHECK(automatic == 1);
    LEARN_CHECK(local_static >= 1);
    g_static = 42;
    LEARN_CHECK(g_static == 42);
}

void demo_intermediate() {
    thread_local int tls = 0;
    ++tls;
    LEARN_CHECK(tls >= 1);
    int* dyn = new int(7);  // dynamic storage
    LEARN_CHECK(*dyn == 7);
    delete dyn;
}

void demo_expert() {
    // Lifetime: storage obtained -> initialization -> use -> destruction -> storage released.
    static int order = 0;
    struct Tracer {
        int id;
        Tracer() : id(++order) {}
        ~Tracer() { /* destroyed at program end for static */ }
    };
    static Tracer t;
    LEARN_CHECK(t.id >= 1);
    int x = 0;
    {
        int y = 1;
        LEARN_CHECK(y == 1);
    }
    // y's lifetime ended; x still alive
    LEARN_CHECK(x == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/storage_duration_phases", run>;

}  // namespace
