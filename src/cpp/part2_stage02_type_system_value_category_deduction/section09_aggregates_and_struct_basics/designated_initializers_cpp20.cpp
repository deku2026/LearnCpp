// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : designated_initializers_cpp20
// Topic id : part2/stage02/section09/designated_initializers_cpp20
//
// Covers: C++20 designated initializers; declaration order; nested designates

#include "learn/topic_registry.hpp"

#include <cassert>

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
    assert(c.width == 800);
    assert(c.height == 600);
    assert(c.fullscreen);

    Config partial{.height = 480};  // width/fullscreen defaulted
    assert(partial.width == 0);
    assert(partial.height == 480);
    assert(!partial.fullscreen);
}

void demo_intermediate() {
    // Designators must follow member declaration order
    Config ok{.width = 1, .height = 2};
    assert(ok.width == 1 && ok.height == 2);

    // Nested designated initialization
    Widget w{.pos = {.x = 10, .y = 20}, .id = 7};
    assert(w.pos.x == 10 && w.pos.y == 20);
    assert(w.id == 7);

    Widget w2{.id = 3};
    assert(w2.pos.x == 0 && w2.pos.y == 0);
    assert(w2.id == 3);
}

void demo_expert() {
    // Mix with arrays of aggregates
    Point path[] = {{.x = 0, .y = 0}, {.x = 1, .y = 1}, {.x = 2, .y = 0}};
    assert(path[1].x == 1 && path[2].y == 0);

    // Out-of-order designators are ill-formed in C++; keep in-order:
    Config c{.width = 1920, .height = 1080, .fullscreen = false};
    assert(c.width == 1920);

    // Designated init is only for aggregates
    auto area = [](const Config& cfg) { return cfg.width * cfg.height; };
    assert(area({.width = 3, .height = 4}) == 12);
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
