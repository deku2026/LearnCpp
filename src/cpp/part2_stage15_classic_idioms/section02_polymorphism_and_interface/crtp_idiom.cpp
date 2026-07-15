// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §2.2
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : crtp_idiom
// Topic id : part2/stage15/section02/crtp_idiom
//
// CRTP (static polymorphism) and C++23 deducing-this simplification.
// Motive : inject behavior / bind calls at compile time without vtable.
// Modern : deducing this removes the "curiously" recursive template on the base.
// Pitfall: CRTP is not runtime polymorphism; heterogeneous containers need virtuals
//          or type erasure.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// ---------- classic CRTP ----------
template <typename Derived>
struct CounterMixin {
    int ticks = 0;

    void tick() {
        ++ticks;
        static_cast<Derived*>(this)->on_tick();
    }

    Derived& self() { return static_cast<Derived&>(*this); }
};

struct Clicker : CounterMixin<Clicker> {
    std::string label = "btn";
    void on_tick() { std::cout << "  [CRTP] " << label << " tick=" << ticks << '\n'; }
};

// Equality helper often shown as CRTP mixin
template <typename Derived>
struct EqualityComparable {
    friend bool operator==(const Derived& a, const Derived& b) { return a.equal_to(b); }
    friend bool operator!=(const Derived& a, const Derived& b) { return !(a == b); }
};

struct Point : EqualityComparable<Point> {
    int x = 0;
    int y = 0;
    bool equal_to(const Point& o) const { return x == o.x && y == o.y; }
};

// ---------- C++23: deducing this static interface (no CRTP template) ----------
struct Animal {
    void make_sound(this auto&& self) {
        // self's type is the derived object — no static_cast, base not a template
        self.make_sound_impl();
    }

    // Fluent API: return type tracks value category of the object parameter.
    template <typename Self>
    auto&& rename(this Self&& self, std::string name) {
        self.name = std::move(name);
        return std::forward<Self>(self);
    }

    std::string name;
};

struct Dog : Animal {
    void make_sound_impl() const { std::cout << "  [deducing this] " << name << ": woof\n"; }
};

struct Cat : Animal {
    void make_sound_impl() const { std::cout << "  [deducing this] " << name << ": meow\n"; }
};

// virtual contrast
struct AnimalV {
    virtual void make_sound() const = 0;
    virtual ~AnimalV() = default;
};
struct DogV : AnimalV {
    void make_sound() const override { std::cout << "  [virtual] woof\n"; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== classic CRTP mixin ==\n";
    Clicker c;
    c.tick();
    c.tick();
    assert(c.ticks == 2);

    Point p1{.x = 1, .y = 2};
    Point p2{.x = 1, .y = 2};
    Point p3{.x = 0, .y = 0};
    const bool same = (p1 == p2);
    const bool diff = (p1 != p3);
    assert(same && diff);
    std::cout << "  EqualityComparable mixin works same=" << same << " diff=" << diff << " (" << p1.x << "," << p1.y
              << ") vs (" << p3.x << "," << p3.y << ")\n";

    std::cout << "== C++23 deducing this (no Base<Derived>) ==\n";
    Dog{}.rename("Rex").make_sound();
    Cat cat;
    cat.rename("Mimi");
    cat.make_sound();

    std::cout << "== runtime polymorphism still needs virtual (or type erasure) ==\n";
    DogV dv;
    AnimalV& ref = dv;
    ref.make_sound();

    std::cout << "prefer deducing this for new static interfaces; keep virtual for runtime sets\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section02/crtp_idiom", run>;

}  // namespace
