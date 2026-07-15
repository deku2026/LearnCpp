// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : deprecated_in_cpp23
// Topic id : part2/stage14/section08/deprecated_in_cpp23
//
// C++23 deprecations (still formable, but discouraged):
//  - std::aligned_storage / std::aligned_union  (P1413) → prefer alignas + byte[]
//  - numeric_limits::has_denorm family          (P2614)
// Demo shows modern replacements as primary path; deprecated API only under
// diagnostic push so -Werror CI can still compile.

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <new>
#include <type_traits>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

#include <type_traits>  // aligned_storage lives here

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace {

// Modern replacement for aligned_storage: alignas + unsigned char / byte buffer
template <std::size_t Len, std::size_t Align>
struct aligned_buffer {
    alignas(Align) unsigned char data[Len];
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++23 deprecated facilities (and replacements) ===\n";

    // --- aligned_storage / aligned_union (deprecated C++23) ---
    {
        // Preferred C++23 style
        aligned_buffer<sizeof(std::uint64_t), alignof(std::uint64_t)> buf{};
        auto* p = std::launder(reinterpret_cast<std::uint64_t*>(buf.data));
        *p = 0x1122334455667788ULL;
        std::cout << "[replacement alignas+buffer] value=0x" << std::hex << *p << std::dec
                  << " align=" << alignof(decltype(buf)) << '\n';
        assert(*p == 0x1122334455667788ULL);

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
        // Deprecated API still present until a future removal
        using Old = std::aligned_storage<sizeof(int), alignof(int)>::type;
        Old old_storage{};
        auto* ip = std::launder(reinterpret_cast<int*>(&old_storage));
        *ip = 42;
        std::cout << "[deprecated aligned_storage] *ip=" << *ip << " (prefer alignas buffer)\n";
        assert(*ip == 42);

        using OldU = std::aligned_union<sizeof(int), int, float>::type;
        std::cout << "[deprecated aligned_union] sizeof=" << sizeof(OldU) << '\n';
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif
    }

    // --- numeric_limits::has_denorm (deprecated C++23, P2614) ---
    {
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
        const auto den = std::numeric_limits<float>::has_denorm;
        const auto denorm_min = std::numeric_limits<float>::denorm_min();
        std::cout << "[deprecated has_denorm] value=" << static_cast<int>(den) << " denorm_min=" << denorm_min
                  << " (has_denorm is deprecated in C++23)\n";
        (void)denorm_min;
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif
        // Practical replacement: use denorm_min() / fp classify APIs rather than has_denorm
        assert(std::numeric_limits<float>::is_iec559);
    }

    std::cout << "deprecated_in_cpp23 demos OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section08/deprecated_in_cpp23", run>;

}  // namespace
