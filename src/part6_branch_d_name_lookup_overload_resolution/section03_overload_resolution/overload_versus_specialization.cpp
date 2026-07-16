// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : overload_versus_specialization
// Topic id : part6/d/section03/overload_versus_specialization
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section03/overload_versus_specialization";

enum class Route { primary, primary_specialization, pointer_overload };

template <class T>
[[nodiscard]] constexpr Route route(T) {
    return Route::primary;
}

// This specializes route(T), but specializations are not overload candidates.
template <>
[[nodiscard]] constexpr Route route<int*>(int*) {
    return Route::primary_specialization;
}

template <class T>
[[nodiscard]] constexpr Route route(T*) {
    return Route::pointer_overload;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int value{};
    // Partial ordering first selects the pointer primary template, so the
    // explicit specialization of the other primary is never considered.
    LEARN_EXPECT(checks, route(&value) == Route::pointer_overload);
    LEARN_EXPECT(checks, route<int*>(&value) == Route::primary_specialization);
    LEARN_EXPECT(checks, route(value) == Route::primary);

    // Function behavior is therefore safer to customize with overloads, ADL,
    // or a constrained customization-point object than with specialization.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section03/overload_versus_specialization", run>;

}  // namespace
