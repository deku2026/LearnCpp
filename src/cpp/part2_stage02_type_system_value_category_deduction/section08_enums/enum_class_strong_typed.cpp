// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : enum_class_strong_typed
// Topic id : part2/stage02/section08/enum_class_strong_typed
//
// Covers: scoped enum class, no implicit int conversion, qualified enumerators

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>

namespace {

enum class Color { Red, Green, Blue };
enum class Priority : int { Low = 1, Medium = 2, High = 3 };

void demo_basics() {
    Color c = Color::Red;
    LEARN_CHECK(c == Color::Red);
    LEARN_CHECK(c != Color::Blue);

    // Color c2 = 0; // ill-formed: no implicit from int
    // int n = c;    // ill-formed: no implicit to int
    int n = static_cast<int>(c);
    LEARN_CHECK(n == 0);
}

void demo_intermediate() {
    Priority p = Priority::High;
    LEARN_CHECK(p == Priority::High);
    LEARN_CHECK(static_cast<int>(p) == 3);

    // Switch on scoped enum
    int score = 0;
    switch (p) {
        case Priority::Low:
            score = 1;
            break;
        case Priority::Medium:
            score = 2;
            break;
        case Priority::High:
            score = 3;
            break;
    }
    LEARN_CHECK(score == 3);

    static_assert(std::is_enum_v<Color>);
#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
    static_assert(std::is_scoped_enum_v<Color>);
#endif
}

void demo_expert() {
    // Distinct enum types do not compare
    // LEARN_CHECK(Color::Red == Priority::Low); // ill-formed

    Color a = Color::Green;
    Color b = Color::Green;
    LEARN_CHECK(a == b);

    // Underlying conversion is explicit only
    auto raw = static_cast<std::underlying_type_t<Priority>>(Priority::Medium);
    LEARN_CHECK(raw == 2);

    // Use as map keys / strong types
    auto name_of = [](Color c) -> const char* {
        switch (c) {
            case Color::Red:
                return "red";
            case Color::Green:
                return "green";
            case Color::Blue:
                return "blue";
        }
        return "unknown";
    };
    LEARN_CHECK(std::string(name_of(Color::Blue)) == "blue");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/enum_class_strong_typed", run>;

}  // namespace
