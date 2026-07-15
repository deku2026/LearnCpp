// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : public_inheritance
// Topic id : part2/stage04/section08/public_inheritance
//
// Covers: public inheritance is-a, base interface reuse, member access

#include "learn/topic_registry.hpp"

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
    LEARN_CHECK(d.name() == "Rex");
    LEARN_CHECK(d.legs() == 4);
    LEARN_CHECK(std::string{d.sound()} == "woof");
}

void demo_intermediate() {
    Dog d{"Fido"};
    Animal& a = d;
    LEARN_CHECK(a.name() == "Fido");
}

void demo_expert() {
    // public inheritance preserves public base interface.
    Dog d{"A"};
    const Animal* p = &d;
    LEARN_CHECK(p->name() == "A");
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
