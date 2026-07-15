// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : cpo_customization_point_object
// Topic id : part6/d/section04/cpo_customization_point_object
//
// Covers: customization point object pattern (niebloid-style)

#include "learn/topic_registry.hpp"

#include <utility>

namespace {

namespace user {
struct Box {
    int v = 0;
};
int tag_invoke_size(Box b) {
    return b.v;
}
}  // namespace user

namespace cpo {
struct size_fn {
    template <class T>
    auto operator()(T&& t) const -> decltype(tag_invoke_size(std::forward<T>(t))) {
        return tag_invoke_size(std::forward<T>(t));
    }
    std::size_t operator()(const char* s) const {
        std::size_t n = 0;
        while (s[n] != '\0') ++n;
        return n;
    }
};
inline constexpr size_fn size{};
}  // namespace cpo

void demo_basics() {
    LEARN_CHECK(cpo::size("hi") == 2);
}

void demo_intermediate() {
    user::Box b{5};
    LEARN_CHECK(cpo::size(b) == 5);
}

void demo_expert() {
    // CPOs are function objects in an inline namespace; users customize via ADL.
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/cpo_customization_point_object", run>;

}  // namespace
