// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : designated_initializers_cpp20
// Topic id : part2/stage02/section09/designated_initializers_cpp20
//
// Covers: C++20 designated initializers; declaration order; nested designates

#include "learn/topic_registry.hpp"

namespace {

struct Config {
    int width = 0;
    int height = 0;
    bool fullscreen = false;
};

struct Point {
    int x = 0;
    int y = 0;
};

struct Widget {
    Point pos{};
    int id = 0;
};

void demo_basics() {
    Config c{.width = 800, .height = 600, .fullscreen = true};
    LEARN_CHECK(c.width == 800);
    LEARN_CHECK(c.height == 600);
    LEARN_CHECK(c.fullscreen);

    Config partial{.height = 480};  // width/fullscreen defaulted
    LEARN_CHECK(partial.width == 0);
    LEARN_CHECK(partial.height == 480);
    LEARN_CHECK(!partial.fullscreen);
}

void demo_intermediate() {
    // Designators must follow member declaration order
    Config ok{.width = 1, .height = 2};
    LEARN_CHECK(ok.width == 1 && ok.height == 2);

    // Nested designated initialization
    Widget w{.pos = {.x = 10, .y = 20}, .id = 7};
    LEARN_CHECK(w.pos.x == 10 && w.pos.y == 20);
    LEARN_CHECK(w.id == 7);

    Widget w2{.id = 3};
    LEARN_CHECK(w2.pos.x == 0 && w2.pos.y == 0);
    LEARN_CHECK(w2.id == 3);
}

void demo_expert() {
    // Mix with arrays of aggregates
    Point path[] = {{.x = 0, .y = 0}, {.x = 1, .y = 1}, {.x = 2, .y = 0}};
    LEARN_CHECK(path[1].x == 1 && path[2].y == 0);

    // Out-of-order designators are ill-formed in C++; keep in-order:
    Config c{.width = 1920, .height = 1080, .fullscreen = false};
    LEARN_CHECK(c.width == 1920);

    // Designated init is only for aggregates
    auto area = [](const Config& cfg) { return cfg.width * cfg.height; };
    LEARN_CHECK(area({.width = 3, .height = 4}) == 12);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section09/designated_initializers_cpp20", run>;

}  // namespace
