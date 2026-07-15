// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : legacy_enum_compare
// Topic id : part2/stage02/section08/legacy_enum_compare
//
// Covers: unscoped enum, implicit int conversion, enumerator leakage, pitfalls

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

enum OldColor { Red, Green, Blue };
enum OldFlags { FlagNone = 0, FlagRead = 1, FlagWrite = 2 };

void demo_basics() {
    OldColor c = Green;
    LEARN_CHECK(c == Green);
    LEARN_CHECK(c != Blue);

    // Implicit conversion to int
    int n = c;
    LEARN_CHECK(n == 1);

    // Compare with int (allowed for unscoped enums — often undesirable)
    LEARN_CHECK(c == 1);
}

void demo_intermediate() {
    // Enumerators pollute the enclosing scope
    int Red_shadow = 99;  // different name; Red already exists as enumerator
    LEARN_CHECK(Red == 0);
    LEARN_CHECK(Red_shadow == 99);

    OldFlags f = FlagRead;
    LEARN_CHECK(f == FlagRead);
    LEARN_CHECK((static_cast<int>(f) | FlagWrite) == 3);

    static_assert(std::is_enum_v<OldColor>);
#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
    static_assert(!std::is_scoped_enum_v<OldColor>);
#endif
}

void demo_expert() {
    // Different unscoped enums convert to int (comparison via ints is the classic pitfall)
    enum A { A0 = 0 };
    enum B { B0 = 0 };
    LEARN_CHECK(static_cast<int>(A0) == static_cast<int>(B0));

    // Prefer enum class to avoid this
    enum class SA { Zero = 0 };
    enum class SB { Zero = 0 };
    LEARN_CHECK(static_cast<int>(SA::Zero) == static_cast<int>(SB::Zero));
    // LEARN_CHECK(SA::Zero == SB::Zero); // ill-formed

    // Switch still works with unscoped enums
    OldColor c = Blue;
    int code = -1;
    switch (c) {
        case Red:
            code = 0;
            break;
        case Green:
            code = 1;
            break;
        case Blue:
            code = 2;
            break;
    }
    LEARN_CHECK(code == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/legacy_enum_compare", run>;

}  // namespace
