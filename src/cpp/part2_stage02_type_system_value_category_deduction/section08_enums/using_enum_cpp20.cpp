// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : using_enum_cpp20
// Topic id : part2/stage02/section08/using_enum_cpp20
//
// Covers: C++20 using enum to inject enumerators into local scope

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

namespace {

enum class Direction { North, South, East, West };
enum class Mode { Read, Write, ReadWrite };

std::string dir_name(Direction d) {
#if defined(__cpp_using_enum) && __cpp_using_enum >= 201907L
    switch (d) {
        using enum Direction;
        case North:
            return "north";
        case South:
            return "south";
        case East:
            return "east";
        case West:
            return "west";
    }
#else
    switch (d) {
        case Direction::North:
            return "north";
        case Direction::South:
            return "south";
        case Direction::East:
            return "east";
        case Direction::West:
            return "west";
    }
#endif
    return "unknown";
}

void demo_basics() {
    LEARN_CHECK(dir_name(Direction::North) == "north");
    LEARN_CHECK(dir_name(Direction::West) == "west");
}

void demo_intermediate() {
#if defined(__cpp_using_enum) && __cpp_using_enum >= 201907L
    {
        using enum Mode;
        Mode m = Read;
        LEARN_CHECK(m == Mode::Read);
        m = Write;
        LEARN_CHECK(m == Write);
    }
#else
    Mode m = Mode::Read;
    LEARN_CHECK(m == Mode::Read);
    m = Mode::Write;
    LEARN_CHECK(m == Mode::Write);
#endif
}

void demo_expert() {
    // Local using enum reduces noise in dense switch logic
    auto opposite = [](Direction d) {
#if defined(__cpp_using_enum) && __cpp_using_enum >= 201907L
        using enum Direction;
        switch (d) {
            case North:
                return South;
            case South:
                return North;
            case East:
                return West;
            case West:
                return East;
        }
#else
        switch (d) {
            case Direction::North:
                return Direction::South;
            case Direction::South:
                return Direction::North;
            case Direction::East:
                return Direction::West;
            case Direction::West:
                return Direction::East;
        }
#endif
        return d;
    };

    LEARN_CHECK(opposite(Direction::North) == Direction::South);
    LEARN_CHECK(opposite(Direction::East) == Direction::West);
    LEARN_CHECK(dir_name(opposite(Direction::South)) == "north");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/using_enum_cpp20", run>;

}  // namespace
