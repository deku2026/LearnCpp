// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : ref_qualified_member_functions
// Topic id : part2/stage04/section06/ref_qualified_member_functions
//
// Covers: & / && ref-qualifiers for lvalue vs rvalue *this

#include "learn/topic_registry.hpp"

#include <string>
#include <utility>

namespace {

class Widget {
    std::string data_;

public:
    explicit Widget(std::string d) : data_(std::move(d)) {}

    const std::string& data() const& { return data_; }
    std::string data() && { return std::move(data_); }

    std::string& data_mut() & { return data_; }
};

void demo_basics() {
    Widget w{"hello"};
    const std::string& r = w.data();
    LEARN_CHECK(r == "hello");
}

void demo_intermediate() {
    std::string s = Widget{"tmp"}.data();
    LEARN_CHECK(s == "tmp");
}

void demo_expert() {
    Widget w{"x"};
    w.data_mut() = "y";
    LEARN_CHECK(w.data() == "y");
    // Four-overload pattern (const/non-const x lvalue/rvalue) is the pain
    // that C++23 deducing this collapses — see section04.
    const Widget cw{"c"};
    LEARN_CHECK(cw.data() == "c");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/ref_qualified_member_functions", run>;

}  // namespace
