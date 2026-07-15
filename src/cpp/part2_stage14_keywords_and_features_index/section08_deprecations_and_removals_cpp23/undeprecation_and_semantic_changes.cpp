// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : undeprecation_and_semantic_changes
// Topic id : part2/stage14/section08/undeprecation_and_semantic_changes
//
// C++23 undeprecations / semantic tightening:
//  - <xxx.h> C headers undeprecated (P2340) — still prefer <cxxx> in pure C++
//  - class layout must follow declaration order (P1847)
//  - volatile compound assignment undeprecated again (C++20 had deprecated it)
//  - comma-in-subscript semantics changed (covered in removed_in_cpp23)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <type_traits>

// C header undeprecated in C++23 (still available without deprecation pressure)
#include <stdlib.h>

namespace {

// P1847: allocation order follows declaration order (even across access labels).
// Use a standard-layout type so offsetof is well-defined for the demo.
struct LayoutDemo {
    char a{};
    int b{};
    char c{};
    int d{};
};

// volatile compound assignment undeprecated in C++23
volatile int g_vol = 0;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++23 undeprecation & semantic changes ===\n";

    // 1) C headers undeprecated — both forms work; show equivalence-ish usage
    {
        void* p = std::malloc(16);
        void* q = malloc(16);  // ::malloc from <stdlib.h>
        assert(p && q);
        std::cout << "[P2340 C headers undeprecated] malloc via <cstdlib> & <stdlib.h> OK\n";
        std::free(p);
        free(q);
        std::cout << "  (idiomatic C++ still prefers <cstdlib> / std::malloc)\n";
    }

    // 2) Declaration-order layout (P1847)
    {
        LayoutDemo ld{};
        const auto off_a = offsetof(LayoutDemo, a);
        const auto off_b = offsetof(LayoutDemo, b);
        const auto off_c = offsetof(LayoutDemo, c);
        const auto off_d = offsetof(LayoutDemo, d);
        std::cout << "[P1847 layout order] a,b,c,d offsets=" << off_a << ',' << off_b << ',' << off_c << ',' << off_d
                  << " sizeof=" << sizeof(LayoutDemo) << '\n';
        assert(off_a < off_b && off_b < off_c && off_c < off_d);
        (void)ld;
        // C++23: allocation order matches declaration order (P1847).
    }

    // 3) volatile compound assignment is not deprecated in C++23
    {
        g_vol = 10;
        g_vol += 5;  // C++20 deprecated; C++23 undeprecated
        g_vol -= 3;
        g_vol *= 2;
        g_vol /= 2;
        std::cout << "[volatile compound op undeprecated] g_vol=" << g_vol << '\n';
        assert(g_vol == 12);
    }

    // 4) Semantic change reminder: [i, j] is multidim call, not comma (see removals topic)
    {
        struct M {
            int operator[](int i, int j) const { return i * 10 + j; }
        };
        M m{};
        std::cout << "[semantic] m[2, 3]=" << m[2, 3] << " (multidim [], not comma)\n";
        assert((m[2, 3] == 23));
    }

    std::cout << "undeprecation_and_semantic_changes OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section08/undeprecation_and_semantic_changes", run>;

}  // namespace
