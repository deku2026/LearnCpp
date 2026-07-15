// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : tuple_and_pair_overview
// Topic id : part3/section03/tuple_and_pair_overview
//
// Covers: std::pair and std::tuple

#include "learn/topic_registry.hpp"

#include <string>
#include <tuple>
#include <utility>

namespace {

void demo_basics() {
    std::pair<int, std::string> p{1, "a"};
    LEARN_CHECK(p.first == 1);
    LEARN_CHECK(p.second == "a");
}

void demo_intermediate() {
    auto t = std::make_tuple(1, 2.5, std::string{"z"});
    LEARN_CHECK(std::get<0>(t) == 1);
    LEARN_CHECK(std::get<2>(t) == "z");
}

void demo_expert() {
    std::tuple<int, int> a{1, 2};
    std::tuple<int, int> b{3, 4};
    auto c = std::tuple_cat(a, b);
    LEARN_CHECK(std::tuple_size_v<decltype(c)> == 4);
    LEARN_CHECK(std::get<3>(c) == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/tuple_and_pair_overview", run>;

}  // namespace
