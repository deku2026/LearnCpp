// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C9 SoA vs AoS)
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : soa_versus_aos
// Topic id : part6/c/section05/soa_versus_aos
//
// 要点: AoS 对象交错；SoA 同字段连续——按访问模式选布局。
// 参考: data-oriented design

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

struct ParticleAoS {
    float x, y, z;
    float mass;
};

struct ParticlesSoA {
    std::vector<float> x, y, z, mass;
    explicit ParticlesSoA(std::size_t n) : x(n), y(n), z(n), mass(n, 1.f) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C9 SoA versus AoS ===\n";

    constexpr std::size_t n = 1024;
    std::vector<ParticleAoS> aos(n);
    for (std::size_t i = 0; i < n; ++i) {
        aos[i] = {float(i), 0.f, 0.f, 1.f};
    }
    [[maybe_unused]] float sum_x_aos = 0;
    for (const auto& p : aos) sum_x_aos += p.x;  // 仍加载 mass 等进入缓存行

    ParticlesSoA soa(n);
    for (std::size_t i = 0; i < n; ++i) soa.x[i] = float(i);
    [[maybe_unused]] float sum_x_soa = std::accumulate(soa.x.begin(), soa.x.end(), 0.f);

    assert(sum_x_aos == sum_x_soa);
    assert(sum_x_soa == float(n * (n - 1) / 2));

    std::cout << "  AoS: great when using whole object\n";
    std::cout << "  SoA: great when streaming one field (SIMD-friendly)\n";
    std::cout << "soa_versus_aos: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/soa_versus_aos", run>;

}  // namespace
