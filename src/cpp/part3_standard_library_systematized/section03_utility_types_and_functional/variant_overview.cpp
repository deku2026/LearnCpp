// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : variant_overview
// Topic id : part3/section03/variant_overview
//
// Covers: std::variant discriminated union

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <variant>

namespace {

void demo_basics() {
    std::variant<int, std::string> v = 10;
    LEARN_CHECK(std::holds_alternative<int>(v));
    LEARN_CHECK(std::get<int>(v) == 10);
    v = std::string{"hi"};
    LEARN_CHECK(std::get<std::string>(v) == "hi");
}

void demo_intermediate() {
    std::variant<int, double> v = 3.5;
    LEARN_CHECK(v.index() == 1);
    LEARN_CHECK(std::get_if<int>(&v) == nullptr);
    LEARN_CHECK(std::get_if<double>(&v) != nullptr);
}

void demo_expert() {
    std::variant<int, std::string> v = 1;
    int sum = 0;
    std::visit(
        [&](auto&& x) {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, int>) {
                sum += x;
            }
        },
        v);
    LEARN_CHECK(sum == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/variant_overview", run>;

}  // namespace
