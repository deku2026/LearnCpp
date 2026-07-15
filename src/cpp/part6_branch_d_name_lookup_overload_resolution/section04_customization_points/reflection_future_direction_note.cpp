// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : reflection_future_direction_note
// Topic id : part6/d/section04/reflection_future_direction_note
//
// Covers: reflection is future direction; today's type traits as stand-in

#include "learn/topic_registry.hpp"

#include <string_view>
#include <type_traits>

namespace {

template <class T>
constexpr std::string_view kind() {
    if constexpr (std::is_integral_v<T>)
        return "integral";
    else if constexpr (std::is_floating_point_v<T>)
        return "floating";
    else
        return "other";
}

void demo_basics() {
    LEARN_CHECK(kind<int>() == "integral");
    LEARN_CHECK(kind<double>() == "floating");
}

void demo_intermediate() {
    LEARN_CHECK(kind<void*>() == "other");
}

void demo_expert() {
    // Future reflection may expose members/enumerators programmatically.
    // Until then: traits, concepts, and code generation tools fill the gap.
    static_assert(std::is_same_v<std::remove_cvref_t<const int&>, int>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/reflection_future_direction_note", run>;

}  // namespace
