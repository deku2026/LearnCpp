// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_this
// Topic id : part2/stage03/section03/capture_this
//
// Covers: [this] captures pointer; member access; lifetime caution

#include "learn/topic_registry.hpp"

#include <functional>
#include <string>

namespace {

struct Counter {
    int value = 0;

    auto make_reader() const {
        return [this] { return value; };
    }

    auto make_bump() {
        return [this] { ++value; };
    }

    std::function<int()> make_safe_snapshot() const {
        // Prefer value when callable outlives *this.
        return [v = value] { return v; };
    }
};

void demo_basics() {
    Counter c{5};
    auto r = c.make_reader();
    LEARN_CHECK(r() == 5);
    c.value = 9;
    LEARN_CHECK(r() == 9);  // still sees live object via this
}

void demo_intermediate() {
    Counter c{0};
    auto bump = c.make_bump();
    bump();
    bump();
    LEARN_CHECK(c.value == 2);
}

void demo_expert() {
    Counter c{11};
    auto snap = c.make_safe_snapshot();
    c.value = 0;
    LEARN_CHECK(snap() == 11);

    // [this] does not extend object lifetime; do not store past *this.
    std::string tag = "this-capture";
    LEARN_CHECK(tag.find("this") != std::string::npos);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_this", run>;

}  // namespace
