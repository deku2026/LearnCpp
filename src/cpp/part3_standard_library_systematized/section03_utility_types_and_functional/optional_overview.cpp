// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : optional_overview
// Topic id : part3/section03/optional_overview
//
// Covers: std::optional value-or-empty

#include "learn/topic_registry.hpp"

#include <cassert>
#include <optional>
#include <string>

namespace {

void demo_basics() {
    std::optional<int> o;
    assert(!o.has_value());
    o = 10;
    assert(o.has_value());
    assert(*o == 10);
}

void demo_intermediate() {
    std::optional<std::string> o = "hi";
    assert(o.value() == "hi");
    assert(o.value_or("x") == "hi");
    o.reset();
    assert(o.value_or("x") == "x");
}

void demo_expert() {
    auto parse = [](bool ok) -> std::optional<int> {
        if (!ok) {
            return std::nullopt;
        }
        return 7;
    };
    assert(parse(true).value() == 7);
    assert(!parse(false));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/optional_overview", run>;

}  // namespace
