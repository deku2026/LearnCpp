// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : conversion_operators
// Topic id : part2/stage04/section05/conversion_operators
//
// Covers: conversion operator to another type, implicit conversion path

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <utility>

namespace {

class Meter {
    double m_ = 0;

public:
    explicit Meter(double m) : m_(m) {}
    operator double() const { return m_; }
    double value() const { return m_; }
};

class Label {
    std::string s_;

public:
    explicit Label(std::string s) : s_(std::move(s)) {}
    operator std::string() const { return s_; }
};

double scale(double x) {
    return x * 2.0;
}

void demo_basics() {
    Meter m{1.5};
    double d = m;
    assert(d == 1.5);
}

void demo_intermediate() {
    Meter m{2.0};
    assert(scale(m) == 4.0);
}

void demo_expert() {
    Label lab{"ok"};
    std::string s = lab;
    assert(s == "ok");
    // Implicit conversion operators can surprise; prefer explicit in new code.
    assert(static_cast<std::string>(lab).size() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/conversion_operators", run>;

}  // namespace
