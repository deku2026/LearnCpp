// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : collapse_ref_qualified_overloads
// Topic id : part2/stage04/section04/collapse_ref_qualified_overloads
// References: [dcl.fct], P0847R7, SD-FeatureTest __cpp_explicit_this_parameter

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section04/collapse_ref_qualified_overloads";

struct Storage {
    int value{};

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    template <class Self>
    decltype(auto) get(this Self&& self) noexcept {
        return (std::forward<Self>(self).value);
    }
#else
    int& get() & noexcept { return value; }
    const int& get() const& noexcept { return value; }
    int&& get() && noexcept { return std::move(value); }
#endif
};

static_assert(std::is_same_v<decltype(std::declval<Storage&>().get()), int&>);
static_assert(std::is_same_v<decltype(std::declval<const Storage&>().get()), const int&>);
static_assert(std::is_same_v<decltype(std::declval<Storage&&>().get()), int&&>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Storage storage{4};
    storage.get() = 9;
    const Storage& view = storage;

    LEARN_EXPECT_EQ(checks, storage.value, 9);
    LEARN_EXPECT_EQ(checks, view.get(), 9);
    // Do not retain the reference returned from a temporary: it would dangle.

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage04/section04/collapse_ref_qualified_overloads", run>;

}  // namespace
