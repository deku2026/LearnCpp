// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : member_template
// Topic id : part2/stage06/section01/member_template
//
// Covers: member function templates, converting constructors across related types

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <utility>
#include <vector>

namespace {

template <typename T>
class Box {
    T value_;

public:
    explicit Box(T v) : value_(std::move(v)) {}

    const T& get() const { return value_; }

    template <typename U>
    explicit Box(const Box<U>& other) : value_(static_cast<T>(other.get())) {}

    template <typename U>
    void assign_from(const Box<U>& other) {
        value_ = static_cast<T>(other.get());
    }
};

struct Holder {
    int x = 0;

    template <typename T>
    void set(T v) {
        x = static_cast<int>(v);
    }

    template <typename T>
    T as() const {
        return static_cast<T>(x);
    }
};

void demo_basics() {
    Box<int> bi(42);
    assert(bi.get() == 42);

    Box<double> bd(bi);
    assert(bd.get() == 42.0);
}

void demo_intermediate() {
    Box<int> bi(10);
    Box<long> bl(0);
    bl.assign_from(bi);
    assert(bl.get() == 10L);

    Holder h;
    h.set(3.9);
    assert(h.x == 3);
    assert(h.as<double>() == 3.0);
}

void demo_expert() {
    // Nested template definition pattern (class-scope member template).
    Box<std::string> bs(std::string{"ok"});
    assert(bs.get() == "ok");

    std::vector<Box<int>> boxes;
    boxes.emplace_back(1);
    boxes.emplace_back(2);
    assert(boxes[0].get() + boxes[1].get() == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/member_template", run>;

}  // namespace
