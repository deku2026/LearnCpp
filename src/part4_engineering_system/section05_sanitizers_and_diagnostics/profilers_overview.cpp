// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : profilers_overview
// Topic id : part4/section05/profilers_overview

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <chrono>
#include <cstdint>
#include <string_view>

namespace {

struct ProfileAccumulator {
    std::chrono::nanoseconds inclusive{};
    std::size_t calls{};
};

class ScopedSample {
public:
    explicit ScopedSample(ProfileAccumulator& profile) noexcept
        : profile_(profile), started_(std::chrono::steady_clock::now()) {}
    ~ScopedSample() noexcept {
        profile_.inclusive +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - started_);
        ++profile_.calls;
    }

    ScopedSample(const ScopedSample&) = delete;
    ScopedSample& operator=(const ScopedSample&) = delete;

private:
    ProfileAccumulator& profile_;
    std::chrono::steady_clock::time_point started_;
};

[[nodiscard]] std::uint64_t workload(std::uint64_t count) noexcept {
    std::uint64_t checksum = 0;
    for (std::uint64_t value = 1; value <= count; ++value) {
        checksum += value;
    }
    return checksum;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/profilers_overview"};

    ProfileAccumulator profile;
    std::uint64_t checksum = 0;
    {
        ScopedSample sample{profile};
        checksum += workload(1'000);
    }
    {
        ScopedSample sample{profile};
        checksum += workload(500);
    }
    LEARN_EXPECT_EQ(checks, checksum, 625'750U);
    LEARN_EXPECT_EQ(checks, profile.calls, 2U);
    LEARN_EXPECT(checks, profile.inclusive.count() >= 0);

    constexpr std::string_view sampling = "perf record -g ./app; perf report (low perturbation, statistical hotspots)";
    constexpr std::string_view instrumentation = "Tracy zones or scoped timers (exact events, some observer overhead)";
    LEARN_EXPECT(checks, sampling.contains("hotspots"));
    LEARN_EXPECT(checks, instrumentation.contains("overhead"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/profilers_overview", run>;

}  // namespace
