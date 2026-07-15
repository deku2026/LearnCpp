// LearnCpp topic
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : new_c_compat_headers
// Topic id : part2/stage14/section08/new_c_compat_headers
//
// C compatibility headers timeline traps (stage14 doc §7.4):
//  - <stdatomic.h> is NEW in C++23 (bridges C _Atomic with std::atomic).
//    There is NO <cstdatomic>.
//  - <stdbit.h> / <stdckdint.h> are C++26 — must NOT be treated as C++23.
//  - Classic <xxx.h> C headers were undeprecated in C++23 (see sibling topic),
//    but idiomatic C++ still prefers <cxxx>.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cstdint>
#include <iostream>
#include <version>

#if __has_include(<stdatomic.h>)
#include <stdatomic.h>
#define LEARN_HAS_STDATOMIC_H 1
#else
#define LEARN_HAS_STDATOMIC_H 0
#endif

// Intentionally NOT including <stdbit.h> / <stdckdint.h> as C++23 requirements.
#if __has_include(<stdbit.h>)
#define LEARN_HAS_STDBIT_H 1
#else
#define LEARN_HAS_STDBIT_H 0
#endif
#if __has_include(<stdckdint.h>)
#define LEARN_HAS_STDCKDINT_H 1
#else
#define LEARN_HAS_STDCKDINT_H 0
#endif

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C++23 C-compat headers checklist ===\n";

    // 1) C++ atomic (always)
    {
        std::atomic<int> a{0};
        a.fetch_add(1, std::memory_order_relaxed);
        std::cout << "[std::atomic] a=" << a.load() << '\n';
        assert(a == 1);
    }

    // 2) <stdatomic.h> (C++23). No <cstdatomic> counterpart.
    std::cout << "[stdatomic.h] __has_include=" << LEARN_HAS_STDATOMIC_H << " (C++23 C compat; NO <cstdatomic>)\n";
#if LEARN_HAS_STDATOMIC_H
    {
        // C11-style atomic via the C header — interoperable intent with std::atomic.
        // On some libcs this provides atomic_int etc.
#if defined(ATOMIC_INT_LOCK_FREE)
        std::cout << "  ATOMIC_INT_LOCK_FREE=" << ATOMIC_INT_LOCK_FREE << '\n';
#endif
        // Prefer std::atomic in C++ code; this cell only proves the header exists.
        std::cout << "  <stdatomic.h> included OK\n";
    }
#endif

    // 3) Emphasize version traps for C++26 headers
    std::cout << "[stdbit.h] __has_include=" << LEARN_HAS_STDBIT_H << " (C++26 — not a C++23 deliverable)\n";
    std::cout << "[stdckdint.h] __has_include=" << LEARN_HAS_STDCKDINT_H
              << " (C++26 checked integer arithmetic — not C++23)\n";

    // 4) Classic C headers still available; C++ form preferred
#if __has_include(<stdlib.h>)
    std::cout << "[stdlib.h] present (C++23 undeprecated C header family)\n";
#endif
#if __has_include(<cstdlib>)
    std::cout << "[cstdlib] preferred C++ wrapper still present\n";
#endif

#if defined(__cpp_lib_stdatomic_h)
    std::cout << "[feature-test] __cpp_lib_stdatomic_h=" << __cpp_lib_stdatomic_h << '\n';
#else
    std::cout << "[feature-test] __cpp_lib_stdatomic_h not defined on this STL\n";
#endif

    std::cout << "new_c_compat_headers OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section08/new_c_compat_headers", run>;

}  // namespace
