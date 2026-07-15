// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : raii_idiom
// Topic id : part2/stage15/section01/raii_idiom
//
// Covers: RAII acquire in ctor, release in dtor

#include "learn/topic_registry.hpp"

#include <cstdlib>

namespace {

struct MallocBuf {
    void* p;
    explicit MallocBuf(std::size_t n) : p(std::malloc(n)) { LEARN_CHECK(p != nullptr); }
    ~MallocBuf() { std::free(p); }
    MallocBuf(const MallocBuf&) = delete;
    MallocBuf& operator=(const MallocBuf&) = delete;
};

void demo_basics() {
    MallocBuf b(64);
    LEARN_CHECK(b.p != nullptr);
}

void demo_intermediate() {
    int freed = 0;
    struct Guard {
        int* f;
        explicit Guard(int* p) : f(p) {}
        ~Guard() { *f = 1; }
    };
    {
        Guard g(&freed);
        LEARN_CHECK(freed == 0);
    }
    LEARN_CHECK(freed == 1);
}

void demo_expert() {
    // Exception path still runs destructor.
    int n = 0;
    try {
        struct G {
            int* p;
            explicit G(int* q) : p(q) {}
            ~G() { *p = 9; }
        } g(&n);
        throw 1;
    } catch (...) {
    }
    LEARN_CHECK(n == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/raii_idiom", run>;

}  // namespace
