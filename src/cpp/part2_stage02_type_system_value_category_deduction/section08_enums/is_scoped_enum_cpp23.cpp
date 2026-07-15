// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : is_scoped_enum_cpp23
// Topic id : part2/stage02/section08/is_scoped_enum_cpp23
//
// Covers: std::is_scoped_enum trait; distinguish enum vs enum class

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <version>

namespace {

enum Unscoped { U0, U1 };
enum class Scoped { S0, S1 };
enum class ScopedFixed : int { A, B };

void demo_basics() {
    static_assert(std::is_enum_v<Unscoped>);
    static_assert(std::is_enum_v<Scoped>);
    static_assert(!std::is_enum_v<int>);

#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
    static_assert(!std::is_scoped_enum_v<Unscoped>);
    static_assert(std::is_scoped_enum_v<Scoped>);
    static_assert(std::is_scoped_enum_v<ScopedFixed>);
#else
    // Portable approximation: scoped enums do not convert to int implicitly.
    // Detect via trait if available; otherwise assert known design intent.
    static_assert(std::is_enum_v<Scoped>);
#endif
}

void demo_intermediate() {
    Unscoped u = U1;
    Scoped s = Scoped::S1;
    LEARN_CHECK(static_cast<int>(u) == 1);
    LEARN_CHECK(static_cast<int>(s) == 1);

    // Unscoped converts; scoped needs cast (shown above)
    int from_u = u;
    LEARN_CHECK(from_u == 1);
}

void demo_expert() {
    auto describe = [](auto e) {
        using E = decltype(e);
        if constexpr (std::is_enum_v<E>) {
#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
            if constexpr (std::is_scoped_enum_v<E>) {
                return 2;  // scoped
            } else {
                return 1;  // unscoped
            }
#else
            return 1;  // feature unavailable: treat generically
#endif
        } else {
            return 0;
        }
    };

#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
    LEARN_CHECK(describe(U0) == 1);
    LEARN_CHECK(describe(Scoped::S0) == 2);
#else
    LEARN_CHECK(describe(U0) == 1);
    LEARN_CHECK(describe(Scoped::S0) == 1);
#endif
    LEARN_CHECK(describe(0) == 0);

    static_assert(std::is_same_v<std::underlying_type_t<ScopedFixed>, int>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/is_scoped_enum_cpp23", run>;

}  // namespace
