// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : full_template_specialization
// Topic id : part2/stage06/section01/full_template_specialization
//
// Covers: full specialization of class/function templates

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <string>
#include <type_traits>

namespace {

template <typename T>
struct TypeName {
    static const char* get() { return "generic"; }
};

template <>
struct TypeName<int> {
    static const char* get() { return "int"; }
};

template <>
struct TypeName<double> {
    static const char* get() { return "double"; }
};

template <typename T>
constexpr int rank_of() {
    return 0;
}

template <>
constexpr int rank_of<void>() {
    return -1;
}

template <>
constexpr int rank_of<std::string>() {
    return 1;
}

void demo_basics() {
    assert(std::strcmp(TypeName<int>::get(), "int") == 0);
    assert(std::strcmp(TypeName<double>::get(), "double") == 0);
    assert(std::strcmp(TypeName<char>::get(), "generic") == 0);
}

void demo_intermediate() {
    static_assert(rank_of<int>() == 0);
    static_assert(rank_of<void>() == -1);
    assert(rank_of<std::string>() == 1);
}

void demo_expert() {
    // Full specialization is a separate definition, not an overload.
    static_assert(std::is_same_v<decltype(TypeName<int>::get()), const char*>);

    struct Custom {};
    assert(std::strcmp(TypeName<Custom>::get(), "generic") == 0);
    assert(rank_of<Custom>() == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/full_template_specialization", run>;

}  // namespace
