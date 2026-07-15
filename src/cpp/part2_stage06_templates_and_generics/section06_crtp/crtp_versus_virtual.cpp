// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_versus_virtual
// Topic id : part2/stage06/section06/crtp_versus_virtual
//
// Covers: CRTP (static) vs virtual (dynamic) polymorphism trade-offs

#include "learn/topic_registry.hpp"

#include <memory>
#include <string>
#include <vector>

namespace {

// Dynamic polymorphism
struct Animal {
    virtual ~Animal() = default;
    virtual std::string speak() const = 0;
};

struct Dog : Animal {
    std::string speak() const override { return "woof"; }
};

struct Cat : Animal {
    std::string speak() const override { return "meow"; }
};

// Static polymorphism (CRTP)
template <typename D>
struct AnimalCrtp {
    std::string speak() const { return static_cast<const D*>(this)->speak_impl(); }
};

struct DogC : AnimalCrtp<DogC> {
    std::string speak_impl() const { return "woof"; }
};

struct CatC : AnimalCrtp<CatC> {
    std::string speak_impl() const { return "meow"; }
};

template <typename D>
std::string loud(const AnimalCrtp<D>& a) {
    return a.speak() + "!";
}

void demo_basics() {
    Dog d;
    Cat c;
    Animal* p = &d;
    LEARN_CHECK(p->speak() == "woof");
    p = &c;
    LEARN_CHECK(p->speak() == "meow");
}

void demo_intermediate() {
    DogC d;
    CatC c;
    LEARN_CHECK(loud(d) == "woof!");
    LEARN_CHECK(loud(c) == "meow!");
}

void demo_expert() {
    // Heterogeneous container needs type erasure / virtual for runtime mix.
    std::vector<std::unique_ptr<Animal>> zoo;
    zoo.push_back(std::make_unique<Dog>());
    zoo.push_back(std::make_unique<Cat>());
    LEARN_CHECK(zoo[0]->speak() == "woof");
    LEARN_CHECK(zoo[1]->speak() == "meow");

    // CRTP is monomorphic per call site — zero virtual dispatch.
    LEARN_CHECK(DogC{}.speak() == "woof");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_versus_virtual", run>;

}  // namespace
