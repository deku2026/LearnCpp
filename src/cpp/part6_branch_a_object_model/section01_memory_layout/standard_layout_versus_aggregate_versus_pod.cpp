// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : standard_layout_versus_aggregate_versus_pod
// Topic id : part6/a/section01/standard_layout_versus_aggregate_versus_pod
//
// Covers: trivial, standard-layout, aggregate, POD deprecation, type traits

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

struct Trivial {
    int x;
    double y;
};

struct WithVirtual {
    virtual void f() {}
    int x = 0;
};

struct MixedAccess {
public:
    int a = 0;
    int get_b() const { return b; }

private:
    int b = 0;
};

struct Aggregate {
    int a;
    int b;
};

struct NonAggregate {
    NonAggregate() = default;
    int a = 0;
};

void demo_basics() {
    static_assert(std::is_trivial_v<Trivial>);
    static_assert(std::is_standard_layout_v<Trivial>);
    static_assert(std::is_trivially_copyable_v<Trivial>);
    static_assert(!std::is_trivial_v<WithVirtual>);
    static_assert(!std::is_standard_layout_v<WithVirtual>);
}

void demo_intermediate() {
    static_assert(!std::is_standard_layout_v<MixedAccess>);
    static_assert(std::is_aggregate_v<Aggregate>);
    static_assert(!std::is_aggregate_v<NonAggregate>);
    Aggregate a{1, 2};
    LEARN_CHECK(a.a == 1 && a.b == 2);
    MixedAccess m;
    LEARN_CHECK(m.a == 0 && m.get_b() == 0);
}

void demo_expert() {
    // POD = trivial + standard-layout; prefer the two orthogonal traits.
    static_assert(std::is_trivial_v<Trivial> && std::is_standard_layout_v<Trivial>);
    // Standard-layout: first member address equals object address.
    Trivial t{3, 4.0};
    LEARN_CHECK(static_cast<void*>(&t) == static_cast<void*>(&t.x));
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section01/standard_layout_versus_aggregate_versus_pod", run>;

}  // namespace
