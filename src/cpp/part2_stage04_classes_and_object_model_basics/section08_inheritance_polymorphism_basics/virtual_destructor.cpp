// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : virtual_destructor
// Topic id : part2/stage04/section08/virtual_destructor
//
// Covers: virtual base dtor required for delete via base pointer

#include "learn/topic_registry.hpp"

#include <memory>

namespace {

int g_base_dtor = 0;
int g_derived_dtor = 0;

class Base {
public:
    virtual ~Base() { ++g_base_dtor; }
};

class Derived : public Base {
public:
    ~Derived() override { ++g_derived_dtor; }
};

class BadBase {
public:
    // Non-virtual dtor: deleting Derived through BadBase* is UB — do not run that.
    ~BadBase() { ++g_base_dtor; }
};

void demo_basics() {
    g_base_dtor = g_derived_dtor = 0;
    {
        Derived d;
    }
    LEARN_CHECK(g_derived_dtor == 1);
    LEARN_CHECK(g_base_dtor == 1);
}

void demo_intermediate() {
    g_base_dtor = g_derived_dtor = 0;
    {
        std::unique_ptr<Base> p = std::make_unique<Derived>();
        LEARN_CHECK(p != nullptr);
    }
    LEARN_CHECK(g_derived_dtor == 1);
    LEARN_CHECK(g_base_dtor == 1);
}

void demo_expert() {
    g_base_dtor = g_derived_dtor = 0;
    Base* p = new Derived();
    delete p;  // safe: Base::~Base is virtual
    LEARN_CHECK(g_derived_dtor == 1);
    LEARN_CHECK(g_base_dtor == 1);

    // Documented unsafe pattern (not executed):
    // BadBase* q = new DerivedAsBadBase(); delete q; // UB without virtual dtor
    (void)sizeof(BadBase);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/virtual_destructor", run>;

}  // namespace
