// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : public_inheritance
// Topic id : part2/stage04/section08/public_inheritance
//
// Covers: public inheritance is-a, base interface reuse, member access

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

class Animal {
    std::string name_;

public:
    explicit Animal(std::string name) : name_(std::move(name)) {}
    const std::string& name() const { return name_; }
    int legs() const { return 4; }
};

class Dog : public Animal {
public:
    explicit Dog(std::string name) : Animal(std::move(name)) {}
    const char* sound() const { return "woof"; }
};

void demo_basics() {
    Dog d{"Rex"};
    assert(d.name() == "Rex");
    assert(d.legs() == 4);
    assert(std::string{d.sound()} == "woof");
}

void demo_intermediate() {
    Dog d{"Fido"};
    Animal& a = d;
    assert(a.name() == "Fido");
}

void demo_expert() {
    // public inheritance preserves public base interface.
    Dog d{"A"};
    const Animal* p = &d;
    assert(p->name() == "A");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/public_inheritance", run>;

}  // namespace
