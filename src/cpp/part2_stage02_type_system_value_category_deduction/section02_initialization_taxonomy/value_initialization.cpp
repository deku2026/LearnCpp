// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : value_initialization
// Topic id : part2/stage02/section02/value_initialization
//
// Covers: T{}, T(), zero for scalars, default ctor then zero for aggregates

#include "learn/topic_registry.hpp"

#include <string>
#include <vector>

namespace {

struct Agg {
    int a;
    double b;
};

struct HasCtor {
    int n;
    HasCtor() : n(5) {}
};

void demo_basics() {
    int i{};
    double d{};
    bool b{};
    int* p{};
    LEARN_CHECK(i == 0);
    LEARN_CHECK(d == 0.0);
    LEARN_CHECK(!b);
    LEARN_CHECK(p == nullptr);

    int j = int();
    LEARN_CHECK(j == 0);
}

void demo_intermediate() {
    Agg a{};
    LEARN_CHECK(a.a == 0);
    LEARN_CHECK(a.b == 0.0);

    HasCtor h{};
    LEARN_CHECK(h.n == 5);  // default ctor runs

    std::string s{};
    LEARN_CHECK(s.empty());

    std::vector<int> v{};
    LEARN_CHECK(v.empty());

    int arr[3]{};
    LEARN_CHECK(arr[0] == 0 && arr[1] == 0 && arr[2] == 0);
}

void demo_expert() {
    // new T() value-initializes; new T default-initializes (unsafe for scalars).
    auto* pi = new int();
    LEARN_CHECK(*pi == 0);
    delete pi;

    auto* pa = new Agg();
    LEARN_CHECK(pa->a == 0 && pa->b == 0.0);
    delete pa;

    // Temporary value-init
    LEARN_CHECK(int{} == 0);
    LEARN_CHECK(double{} == 0.0);

    struct Nested {
        Agg inner;
        int tag;
    };
    Nested n{};
    LEARN_CHECK(n.inner.a == 0 && n.inner.b == 0.0 && n.tag == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/value_initialization", run>;

}  // namespace
