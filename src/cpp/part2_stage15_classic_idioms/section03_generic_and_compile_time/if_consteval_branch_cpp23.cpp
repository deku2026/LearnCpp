// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §3.2 (if consteval)
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : if_consteval_branch_cpp23
// Topic id : part2/stage15/section03/if_consteval_branch_cpp23
//
// if consteval (C++23): branch on whether the call is in a constant-evaluation context.
// Motive : one function can use compile-time algorithms at consteval time and
//          runtime-friendly code otherwise (e.g. logging, asm, syscalls).
// Modern : replaces awkward is_constant_evaluated patterns for many dual-path APIs.
// Pitfall: if consteval has no condition; do not confuse with if constexpr (type/value).

#include "learn/topic_registry.hpp"

#include <cstdlib>
#include <iostream>
#include <type_traits>

namespace {

// Dual path: heavy runtime math vs simple compile-time formula.
constexpr int scale(int x) {
    if consteval {
        // compile-time: only constexpr-friendly ops
        return x * 100;
    } else {
        // runtime: could call non-constexpr helpers, log, etc.
        std::cout << "  [runtime path] scale(" << x << ")\n";
        return x;
    }
}

constexpr int abs_diff(int a, int b) {
    if consteval {
        return a > b ? a - b : b - a;
    } else {
        // illustrate runtime-only facility
        return static_cast<int>(std::abs(a - b));
    }
}

// Pre-C++23 style contrast using is_constant_evaluated.
constexpr int scale_old(int x) {
    if (std::is_constant_evaluated()) {
        return x * 100;
    }
    // Note: both arms must be valid in a constexpr function more carefully;
    // is_constant_evaluated is a plain if, not a syntactic split like if consteval.
    return x;
}

// if constexpr vs if consteval reminder
template <typename T>
constexpr int pick(T v) {
    if constexpr (std::is_integral_v<T>) {
        // chosen by type at instantiation time
        if consteval {
            return static_cast<int>(v) + 1000;  // chosen by evaluation context
        } else {
            return static_cast<int>(v);
        }
    } else {
        return -1;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== if consteval dual path ==\n";
    constexpr int ct = scale(3);  // constant evaluation → *100
    static_assert(ct == 300);
    std::cout << "  compile-time scale(3)=" << ct << '\n';

    int rt = scale(3);  // runtime evaluation → identity (+ print)
    assert(rt == 3);
    std::cout << "  runtime scale(3)=" << rt << '\n';

    constexpr int d1 = abs_diff(10, 3);
    static_assert(d1 == 7);
    const int d2 = abs_diff(10, 3);
    assert(d2 == 7);
    std::cout << "  abs_diff compile=" << d1 << " runtime=" << d2 << '\n';

    std::cout << "== old is_constant_evaluated ==\n";
    constexpr int old_ct = scale_old(2);
    static_assert(old_ct == 200);
    assert(scale_old(2) == 2);
    std::cout << "  scale_old ct=200 rt=2\n";

    std::cout << "== combine if constexpr + if consteval ==\n";
    constexpr int p = pick(5);
    static_assert(p == 1005);
    assert(pick(5) == 5);
    std::cout << "  pick(5) ct=" << p << " rt=" << pick(5) << '\n';

    std::cout << "if constexpr → type/constant condition; if consteval → evaluation context\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/if_consteval_branch_cpp23", run>;

}  // namespace
