// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : reflection_future_direction_note
// Topic id : part6/d/section04/reflection_future_direction_note
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <array>
#include <concepts>
#include <functional>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/d/section04/reflection_future_direction_note";

struct Record {
    int id{};
    std::string_view name;
};

template <class T>
concept Named = requires(const T& value) {
    { value.name } -> std::convertible_to<std::string_view>;
};

// C++23 has no standardized static-reflection syntax. Explicit metadata,
// concepts, traits, tuple protocol, and member pointers are portable options.
constexpr auto record_members = std::tuple{&Record::id, &Record::name};

static_assert(std::is_aggregate_v<Record>);
static_assert(Named<Record>);
static_assert(std::tuple_size_v<decltype(record_members)> == 2);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const Record record{7, "Ada"};
    LEARN_EXPECT_EQ(checks, std::invoke(std::get<0>(record_members), record), 7);
    LEARN_EXPECT_EQ(checks, std::invoke(std::get<1>(record_members), record), std::string_view{"Ada"});

#if 0
    // Future reflection proposals must not be guessed into production code.
    // Use only the syntax and feature-test macro of the finally adopted standard.
    constexpr auto metadata = reflexpr(Record);  // deliberately inactive/non-standard
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section04/reflection_future_direction_note", run>;

}  // namespace
