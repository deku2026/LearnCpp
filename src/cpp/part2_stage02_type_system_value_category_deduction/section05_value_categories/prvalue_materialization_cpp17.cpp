// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section05_value_categories
// Item     : prvalue_materialization_cpp17
// Topic id : part2/stage02/section05/prvalue_materialization_cpp17
//
// Covers: C++17 prvalue as initialization; temporary materialization conversion

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>

namespace {

struct Tracker {
    int id;
    explicit Tracker(int v) : id(v) {}
};

Tracker make_tracker(int v) {
    return Tracker{v};
}  // prvalue return

void demo_basics() {
    // C++17: initializing from prvalue initializes the target directly (GUARANTEED elision).
    Tracker t = make_tracker(7);
    LEARN_CHECK(t.id == 7);

    Tracker t2 = Tracker{8};
    LEARN_CHECK(t2.id == 8);
}

void demo_intermediate() {
    // Temporary materialization: prvalue becomes xvalue when a glvalue is needed.
    const Tracker& ref = Tracker{9};  // materializes temporary, binds const ref
    LEARN_CHECK(ref.id == 9);

    // Member access on prvalue materializes then accesses
    LEARN_CHECK(Tracker{10}.id == 10);

    // string prvalue materialization for const ref parameter pattern
    auto len = [](const std::string& s) { return s.size(); };
    LEARN_CHECK(len(std::string("abcd")) == 4);
}

void demo_expert() {
    // decltype of a prvalue call is the value type, not a reference
    static_assert(std::is_same_v<decltype(make_tracker(1)), Tracker>);
    static_assert(!std::is_reference_v<decltype(make_tracker(1))>);

    // Nested construction still yields the final object value
    auto factory = [](int n) { return make_tracker(n + 1); };
    Tracker t = factory(40);
    LEARN_CHECK(t.id == 41);

    // Array prvalue is not common; use brace init materialization via const ref
    const int (&arr_ref)[3] = {1, 2, 3};
    LEARN_CHECK(arr_ref[0] == 1 && arr_ref[2] == 3);

    // Mandatory elision means even non-copyable types can return by value
    struct MoveOnly {
        int v;
        explicit MoveOnly(int x) : v(x) {}
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly(MoveOnly&&) = default;
        MoveOnly& operator=(const MoveOnly&) = delete;
        MoveOnly& operator=(MoveOnly&&) = default;
    };
    auto make_mo = [] { return MoveOnly{3}; };
    MoveOnly mo = make_mo();
    LEARN_CHECK(mo.v == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/prvalue_materialization_cpp17", run>;

}  // namespace
