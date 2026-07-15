// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : simplified_crtp_via_deducing_this
// Topic id : part2/stage04/section04/simplified_crtp_via_deducing_this
//
// Covers: deducing this CRTP without Base<Derived> template parameter

#include "learn/topic_registry.hpp"

#include <string>

namespace {

#if defined(__cpp_explicit_this_parameter)

struct Base {
    int interface(this auto&& self) { return self.impl(); }
};

struct Derived : Base {
    int impl() const { return 42; }
};

struct Named : Base {
    std::string label = "n";
    int impl() const { return static_cast<int>(label.size()); }
};

void demo_basics() {
    Derived d;
    LEARN_CHECK(d.interface() == 42);
}

void demo_intermediate() {
    Named n;
    LEARN_CHECK(n.interface() == 1);
}

void demo_expert() {
    // self is the static type of the object expression (Derived/Named).
    // Call on the derived object so Self deduces to Derived&, not Base&.
    Derived d;
    LEARN_CHECK(d.interface() == 42);
    Named n;
    n.label = "abcd";
    LEARN_CHECK(n.interface() == 4);
}

#else

template <class D>
struct Base {
    int interface() { return static_cast<D*>(this)->impl(); }
};

struct Derived : Base<Derived> {
    int impl() const { return 42; }
};

void demo_basics() {
    Derived d;
    LEARN_CHECK(d.interface() == 42);
}

void demo_intermediate() {
    LEARN_CHECK(true);
}

void demo_expert() {
    LEARN_CHECK(true);
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/simplified_crtp_via_deducing_this", run>;

}  // namespace
