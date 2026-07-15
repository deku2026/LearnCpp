// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : deducing_this_restrictions
// Topic id : part2/stage04/section04/deducing_this_restrictions
//
// Covers: cannot be static/virtual/cv-ref-qualified; no this pointer inside

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

#if defined(__cpp_explicit_this_parameter)

struct Demo {
    int n = 1;

    int read(this const Demo& self) { return self.n; }

    // Ill-formed patterns (documented only, not compiled):
    // static void bad_static(this Demo&);
    // virtual void bad_virtual(this Demo&);
    // void bad_cv(this Demo& self) const;
    // void bad_this(this Demo& self) { (void)this; }
};

struct Poly {
    virtual int f() const { return 1; }
    virtual ~Poly() = default;
};

struct Child : Poly {
    int f() const override { return 2; }
    int via_explicit(this const Child& self) { return self.f(); }
};

void demo_basics() {
    Demo d;
    assert(d.read() == 1);
}

void demo_intermediate() {
    // Virtual dispatch stays on ordinary virtual members; explicit-object
    // members are separate and non-virtual by rule.
    Child c;
    Poly& p = c;
    assert(p.f() == 2);
    assert(c.via_explicit() == 2);
}

void demo_expert() {
    // Explicit object params carry cv/ref through the parameter type itself.
    Demo d;
    const Demo& r = d;
    assert(r.read() == 1);
    static_assert(std::is_invocable_v<decltype(&Demo::read), const Demo&>);
}

#else

void demo_basics() {
    assert(true);
}

void demo_intermediate() {
    assert(true);
}

void demo_expert() {
    // Restrictions apply only when the feature exists.
    assert(true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/deducing_this_restrictions", run>;

}  // namespace
