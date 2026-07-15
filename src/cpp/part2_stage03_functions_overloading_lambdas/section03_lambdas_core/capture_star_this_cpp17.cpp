// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_star_this_cpp17
// Topic id : part2/stage03/section03/capture_star_this_cpp17
//
// Covers: [*this] copies the object into the closure (C++17)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

struct Widget {
    int id = 0;
    std::string name = "w";

    auto by_this() const {
        return [this] { return id; };
    }

    auto by_star_this() const {
        return [*this] { return id; };
    }

    auto name_snapshot() const {
        return [*this] { return name; };
    }
};

void demo_basics() {
    Widget w{7, "alpha"};
    auto f = w.by_star_this();
    w.id = 0;
    assert(f() == 7);
}

void demo_intermediate() {
    Widget w{1, "beta"};
    auto ptr_view = w.by_this();
    auto copy_view = w.by_star_this();
    w.id = 99;
    assert(ptr_view() == 99);
    assert(copy_view() == 1);
}

void demo_expert() {
    Widget w{3, "gamma"};
    auto name_fn = w.name_snapshot();
    w.name = "changed";
    assert(name_fn() == "gamma");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_star_this_cpp17", run>;

}  // namespace
