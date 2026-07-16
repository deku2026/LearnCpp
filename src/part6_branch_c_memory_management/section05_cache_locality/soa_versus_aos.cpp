// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : soa_versus_aos
// Topic id : part6/c/section05/soa_versus_aos
// Reference: Data-oriented layout example; both layouts use standard contiguous vectors.

#include "learn/example_support.hpp"

#include <cstddef>
#include <vector>

namespace {

struct Particle {
    float x;
    float y;
    float mass;
};

struct ParticleColumns {
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> mass;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section05/soa_versus_aos";
    learn::ExampleChecks checks{topic};

    std::vector<Particle> aos{{1.0F, 2.0F, 3.0F}, {4.0F, 5.0F, 6.0F}, {7.0F, 8.0F, 9.0F}};
    ParticleColumns soa{{1.0F, 4.0F, 7.0F}, {2.0F, 5.0F, 8.0F}, {3.0F, 6.0F, 9.0F}};

    float aos_mass{};
    for (const Particle& particle : aos) {
        aos_mass += particle.mass;
    }
    float soa_mass{};
    for (const float mass : soa.mass) {
        soa_mass += mass;
    }
    LEARN_EXPECT_EQ(checks, aos_mass, 18.0F);
    LEARN_EXPECT_EQ(checks, soa_mass, aos_mass);

    for (Particle& particle : aos) {
        particle.x += 0.5F;
    }
    for (float& x : soa.x) {
        x += 0.5F;
    }
    for (std::size_t index = 0; index < aos.size(); ++index) {
        LEARN_EXPECT_EQ(checks, aos[index].x, soa.x[index]);
    }

    // AoS keeps all fields of one entity together; SoA streams one field densely and can simplify
    // SIMD/vectorization. Hybrid/AoSoA layouts are often better when algorithms consume field groups.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/soa_versus_aos", run>;

}  // namespace
