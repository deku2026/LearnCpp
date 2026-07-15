// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : soa_versus_aos
// Topic id : part6/c/section05/soa_versus_aos
//
// Covers: structure of arrays vs array of structures for locality

#include "learn/topic_registry.hpp"

#include <vector>

namespace {

struct AoS {
    float x, y, z;
};

struct SoA {
    std::vector<float> x, y, z;
};

void demo_basics() {
    std::vector<AoS> particles{{1, 0, 0}, {2, 0, 0}};
    float sum = 0;
    for (const auto& p : particles) sum += p.x;
    LEARN_CHECK(sum == 3.f);
}

void demo_intermediate() {
    SoA s;
    s.x = {1, 2, 3};
    s.y = {0, 0, 0};
    s.z = {0, 0, 0};
    float sum = 0;
    for (float v : s.x) sum += v;
    LEARN_CHECK(sum == 6.f);
}

void demo_expert() {
    // SoA: hot field x contiguous; better when only x is streamed.
    SoA s;
    s.x.assign(100, 1.f);
    float sum = 0;
    for (float v : s.x) sum += v;
    LEARN_CHECK(sum == 100.f);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/soa_versus_aos", run>;

}  // namespace
