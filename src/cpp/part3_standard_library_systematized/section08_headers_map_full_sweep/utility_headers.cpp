// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : utility_headers
// Topic id : part3/section08/utility_headers
//
// Covers: headers map: utility tuple optional variant any functional

#include "learn/topic_registry.hpp"

#include <any>
#include <cassert>
#include <functional>
#include <optional>
#include <tuple>
#include <utility>
#include <variant>

namespace {

void demo_basics() {
    auto p = std::make_pair(1, 2);
    assert(p.first == 1);
}

void demo_intermediate() {
    std::optional<int> o = 3;
    assert(*o == 3);
    std::variant<int, double> v = 1.5;
    assert(std::holds_alternative<double>(v));
}

void demo_expert() {
    std::function<int()> f = [] { return 1; };
    assert(f() == 1);
    std::any a = 9;
    assert(std::any_cast<int>(a) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/utility_headers", run>;

}  // namespace
