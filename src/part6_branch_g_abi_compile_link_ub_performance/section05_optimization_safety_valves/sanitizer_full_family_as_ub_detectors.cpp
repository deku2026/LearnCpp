// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : sanitizer_full_family_as_ub_detectors
// Topic id : part6/g/section05/sanitizer_full_family_as_ub_detectors

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <string_view>

namespace {

#if defined(__clang__)
#if __has_feature(address_sanitizer)
constexpr bool asan_active = true;
#else
constexpr bool asan_active = false;
#endif
#if __has_feature(undefined_behavior_sanitizer)
constexpr bool ubsan_active = true;
#else
constexpr bool ubsan_active = false;
#endif
#if __has_feature(thread_sanitizer)
constexpr bool tsan_active = true;
#else
constexpr bool tsan_active = false;
#endif
#if __has_feature(memory_sanitizer)
constexpr bool msan_active = true;
#else
constexpr bool msan_active = false;
#endif
#if __has_feature(leak_sanitizer)
constexpr bool lsan_active = true;
#else
constexpr bool lsan_active = false;
#endif
#else
#if defined(__SANITIZE_ADDRESS__)
constexpr bool asan_active = true;
#else
constexpr bool asan_active = false;
#endif
#if defined(__SANITIZE_UNDEFINED__)
constexpr bool ubsan_active = true;
#else
constexpr bool ubsan_active = false;
#endif
#if defined(__SANITIZE_THREAD__)
constexpr bool tsan_active = true;
#else
constexpr bool tsan_active = false;
#endif
constexpr bool msan_active = false;
constexpr bool lsan_active = false;
#endif

struct Detector {
    std::string_view name;
    std::string_view finds;
    bool active;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section05/sanitizer_full_family_as_ub_detectors"};

    constexpr std::array detectors{
        Detector{"ASan", "bounds, use-after-free", asan_active},
        Detector{"UBSan", "language undefined behavior", ubsan_active},
        Detector{"TSan", "data races", tsan_active},
        Detector{"MSan", "uninitialized reads", msan_active},
        Detector{"LSan", "leaks", lsan_active},
    };
    static_assert(detectors.size() == 5);
    LEARN_EXPECT_EQ(checks, detectors[0].name, std::string_view{"ASan"});
    LEARN_EXPECT(checks, detectors[2].finds.contains("races"));
    int active_count = 0;
    for (const auto& detector : detectors) {
        active_count += detector.active ? 1 : 0;
        LEARN_EXPECT(checks, !detector.finds.empty());
    }
    LEARN_EXPECT(checks, active_count >= 0 && active_count <= 5);

#if 0
    // Put one defect per expected-failure subprocess; never run these in the
    // normal topic sweep, and keep TSan/MSan in dedicated incompatible lanes.
    int out_of_bounds[1]{};
    out_of_bounds[1] = 7;
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/sanitizer_full_family_as_ub_detectors", run>;

}  // namespace
