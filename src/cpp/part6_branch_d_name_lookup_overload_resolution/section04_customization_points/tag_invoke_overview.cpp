// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : tag_invoke_overview
// Topic id : part6/d/section04/tag_invoke_overview
//
// Covers: tag_invoke customization mechanism overview

#include "learn/topic_registry.hpp"

#include <cassert>
#include <utility>

namespace {

namespace lib {
struct tag_t {};
inline constexpr tag_t tag{};

template <class T>
auto tag_invoke(tag_t, const T& t) -> decltype(t.primary()) {
    return t.primary();
}

template <class T>
auto invoke_tag(const T& t) -> decltype(tag_invoke(tag, t)) {
    return tag_invoke(tag, t);
}
}  // namespace lib

namespace user {
struct Service {
    int primary() const { return 11; }
};
// Alternative customization:
// int tag_invoke(lib::tag_t, const Service&) { return 99; }
}  // namespace user

void demo_basics() {
    user::Service s;
    assert(lib::invoke_tag(s) == 11);
}

struct Alt {
    friend int tag_invoke(lib::tag_t, const Alt&) { return 7; }
};

void demo_intermediate() {
    assert(lib::invoke_tag(Alt{}) == 7);
}

void demo_expert() {
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/tag_invoke_overview", run>;

}  // namespace
