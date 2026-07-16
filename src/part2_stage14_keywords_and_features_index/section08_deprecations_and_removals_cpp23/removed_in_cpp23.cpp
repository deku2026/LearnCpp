// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : removed_in_cpp23
// Topic id : part2/stage14/section08/removed_in_cpp23
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <memory>
#include <string_view>
#include <type_traits>
#include <variant>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section08/removed_in_cpp23";

struct Removal {
    std::string_view facility;
    std::string_view replacement;
};

constexpr std::array removals{
    Removal{"garbage-collection reachability API", "deterministic RAII ownership"},
    Removal{"volatile tuple helper specializations", "synchronization primitives/atomics"},
    Removal{"volatile variant helper specializations", "synchronization primitives/atomics"},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto owner = std::make_unique<int>(23);
    const std::variant<int, std::string_view> value{*owner};
    LEARN_EXPECT_EQ(checks, std::get<int>(value), 23);
    LEARN_EXPECT_EQ(checks, removals.size(), 3U);
    // Removed standard names may remain as implementation "zombie names"; portable code cannot rely on them.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage14/section08/removed_in_cpp23", run>;

}  // namespace
