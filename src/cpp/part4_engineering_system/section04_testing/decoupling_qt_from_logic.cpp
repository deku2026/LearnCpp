// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : decoupling_qt_from_logic
// Topic id : part4/section04/decoupling_qt_from_logic
//
// Covers: keep domain logic free of QWidget for testability

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Domain: plain C++ types; UI: adapters
    struct Order {
        int total;
    };
    Order o{10};
    LEARN_CHECK(o.total == 10);
}

void demo_intermediate() {
    // Test Order without QApplication
    LEARN_CHECK(true);
}

void demo_expert() {
    // Hexagonal/clean architecture boundaries
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/decoupling_qt_from_logic", run>;

}  // namespace
