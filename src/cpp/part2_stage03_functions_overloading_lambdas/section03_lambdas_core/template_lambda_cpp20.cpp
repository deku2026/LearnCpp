// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : template_lambda_cpp20
// Topic id : part2/stage03/section03/template_lambda_cpp20
//
// Covers: C++20 explicit template parameter list on lambdas

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <vector>

namespace {

void demo_basics() {
    auto first_of = []<class T>(const std::vector<T>& v) -> T { return v.empty() ? T{} : v.front(); };
    std::vector<int> v{10, 20, 30};
    LEARN_CHECK(first_of(v) == 10);
}

void demo_intermediate() {
    auto size_of_elem = []<class T>(const std::vector<T>&) { return sizeof(T); };
    std::vector<int> vi{1};
    std::vector<double> vd{1.0};
    LEARN_CHECK(size_of_elem(vi) == sizeof(int));
    LEARN_CHECK(size_of_elem(vd) == sizeof(double));
}

void demo_expert() {
    auto cast_add = []<class T>(T a, T b) { return static_cast<T>(a + b); };
    LEARN_CHECK(cast_add(1, 2) == 3);
    static_assert(std::is_same_v<decltype(cast_add(1, 2)), int>);

    auto id = []<class T>(T x) -> T { return x; };
    LEARN_CHECK(id(std::string{"ok"}) == "ok");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/template_lambda_cpp20", run>;

}  // namespace
