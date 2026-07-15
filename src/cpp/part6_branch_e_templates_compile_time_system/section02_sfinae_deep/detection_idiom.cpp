// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section02_sfinae_deep
// Item     : detection_idiom
// Topic id : part6/e/section02/detection_idiom
//
// Covers: is_detected-style detection

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>

namespace {

template <class T>
using serialize_t = decltype(std::declval<T>().serialize());

template <class, class = void>
struct is_serializable : std::false_type {};

template <class T>
struct is_serializable<T, std::void_t<serialize_t<T>>> : std::true_type {};

struct A {
    int serialize() const { return 1; }
};
struct B {};

void demo_basics() {
    static_assert(is_serializable<A>::value);
    static_assert(!is_serializable<B>::value);
}

void demo_intermediate() {
    A a;
    LEARN_CHECK(a.serialize() == 1);
}

void demo_expert() {
    LEARN_CHECK(is_serializable<A>::value);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section02/detection_idiom", run>;

}  // namespace
