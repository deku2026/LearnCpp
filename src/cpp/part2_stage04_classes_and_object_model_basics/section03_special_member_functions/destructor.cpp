// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : destructor
// Topic id : part2/stage04/section03/destructor
//
// Covers: dtor order reverse of construction, cleanup, noexcept convention

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

int g_alive = 0;

struct Tracer {
    int id = 0;
    explicit Tracer(int i) : id(i) { ++g_alive; }
    ~Tracer() { --g_alive; }
};

class Box {
    Tracer t_;
    std::string label_;

public:
    Box(int id, std::string label) : t_(id), label_(std::move(label)) {}
    ~Box() = default;

    int id() const { return t_.id; }
    const std::string& label() const { return label_; }
};

void demo_basics() {
    g_alive = 0;
    {
        Tracer a{1};
        assert(g_alive == 1);
    }
    assert(g_alive == 0);
}

void demo_intermediate() {
    g_alive = 0;
    {
        Box b{3, "x"};
        assert(b.id() == 3);
        assert(b.label() == "x");
        assert(g_alive == 1);
    }
    assert(g_alive == 0);
}

void demo_expert() {
    g_alive = 0;
    {
        std::vector<Tracer> v;
        v.reserve(2);  // avoid reallocation (default copy/move would not bump g_alive)
        v.emplace_back(1);
        v.emplace_back(2);
        assert(g_alive == 2);
    }
    assert(g_alive == 0);

    // Destructors should not throw under normal RAII design.
    static_assert(std::is_nothrow_destructible_v<Box>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/destructor", run>;

}  // namespace
