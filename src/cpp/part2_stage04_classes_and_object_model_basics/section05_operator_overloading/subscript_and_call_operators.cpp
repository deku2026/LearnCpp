// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : subscript_and_call_operators
// Topic id : part2/stage04/section05/subscript_and_call_operators
//
// Covers: operator[] const/non-const, operator() function object

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <vector>

namespace {

class IntArray {
    std::vector<int> data_;

public:
    explicit IntArray(std::size_t n) : data_(n) {}
    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }
    std::size_t size() const { return data_.size(); }
};

class Multiplier {
    int factor_;

public:
    explicit Multiplier(int f) : factor_(f) {}
    int operator()(int x) const { return x * factor_; }
};

void demo_basics() {
    IntArray a{3};
    a[0] = 10;
    a[1] = 20;
    LEARN_CHECK(a[0] == 10);
    LEARN_CHECK(a.size() == 3);
}

void demo_intermediate() {
    const IntArray a = [] {
        IntArray t{2};
        t[0] = 1;
        t[1] = 2;
        return t;
    }();
    LEARN_CHECK(a[1] == 2);
}

void demo_expert() {
    Multiplier times3{3};
    LEARN_CHECK(times3(7) == 21);
    auto apply = [](const Multiplier& m, int x) { return m(x); };
    LEARN_CHECK(apply(times3, 4) == 12);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/subscript_and_call_operators", run>;

}  // namespace
