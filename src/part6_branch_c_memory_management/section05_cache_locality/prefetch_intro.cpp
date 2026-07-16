// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : prefetch_intro
// Topic id : part6/c/section05/prefetch_intro
// Reference: Prefetch is an implementation extension; the C++ abstract machine has no API.

#include "learn/example_support.hpp"

#include <cstddef>
#include <cstdint>
#include <numeric>
#include <vector>

#if defined(_MSC_VER) && !defined(__clang__)
#include <immintrin.h>
#endif

namespace {

void prefetch_for_read(const int* address) noexcept {
#if defined(__clang__) || defined(__GNUC__)
    __builtin_prefetch(address, 0, 1);
#elif defined(_MSC_VER)
    _mm_prefetch(reinterpret_cast<const char*>(address), _MM_HINT_T0);
#else
    (void)address;
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section05/prefetch_intro";
    learn::ExampleChecks checks{topic};

    std::vector<int> values(256);
    std::iota(values.begin(), values.end(), 1);
    std::int64_t sum{};
    constexpr std::size_t look_ahead = 16;
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index + look_ahead < values.size()) {
            prefetch_for_read(&values[index + look_ahead]);
        }
        sum += values[index];
    }
    const auto expected = std::accumulate(values.begin(), values.end(), std::int64_t{0});
    LEARN_EXPECT_EQ(checks, sum, expected);

    // A prefetch is merely a compiler/CPU hint: it must not change results, replace bounds checks,
    // or be assumed to complete. Sequential hardware prefetchers often make manual hints slower;
    // retain them only after representative profiling demonstrates a benefit.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/prefetch_intro", run>;

}  // namespace
