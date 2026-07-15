// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C9 SoA vs AoS)
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : soa_versus_aos
// Topic id : part6/c/section05/soa_versus_aos
//
// 要点: AoS 利于整对象；SoA 利于单字段批量扫描（SIMD/缓存）。
// 参考: data-oriented design

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <vector>

namespace {

struct ParticleAoS {
    float x, y, z;
    float vx, vy, vz;
};

struct ParticlesSoA {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    explicit ParticlesSoA(std::size_t n) : x(n), y(n), z(n), vx(n), vy(n), vz(n) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C9 SoA versus AoS ===\n";

    constexpr int n = 1000;
    std::vector<ParticleAoS> aos(n);
    for (int i = 0; i < n; ++i) {
        aos[i] = ParticleAoS{1.f, 2.f, 3.f, 0.1f, 0.f, 0.f};
    }
    // 只更新 x：仍可能拖入整个结构缓存行
    for (auto& p : aos) p.x += p.vx;
    assert(aos[0].x > 1.f);

    ParticlesSoA soa(n);
    for (int i = 0; i < n; ++i) {
        soa.x[i] = 1.f;
        soa.vx[i] = 0.1f;
        soa.y[i] = 2.f;
    }
    for (int i = 0; i < n; ++i) soa.x[i] += soa.vx[i];
    assert(soa.x[0] > 1.f);
    // y 未触碰但 SoA 下 x 扫描更紧凑

    std::cout << "  AoS: whole-object ops; SoA: field-wise bulk scans\n";
    std::cout << "soa_versus_aos: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/soa_versus_aos", run>;

}  // namespace
