// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : deprecated_in_cpp23
// Topic id : part2/stage14/section08/deprecated_in_cpp23
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <new>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section08/deprecated_in_cpp23";

struct Migration {
    std::string_view deprecated_facility;
    std::string_view preferred_direction;
};

constexpr std::array migrations{
    Migration{"std::aligned_storage", "alignas(T) std::byte storage[sizeof(T)]"},
    Migration{"std::aligned_union", "explicitly aligned byte storage or variant"},
    Migration{"std::is_pod", "is_trivial and/or is_standard_layout for the real requirement"},
    Migration{"polymorphic_allocator::destroy", "allocator_traits::destroy"},
};

struct Payload {
    int value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    alignas(Payload) std::byte storage[sizeof(Payload)]{};
    auto* payload = ::new (static_cast<void*>(storage)) Payload{23};
    LEARN_EXPECT_EQ(checks, payload->value, 23);
    std::destroy_at(payload);
    static_assert(std::is_trivial_v<Payload> && std::is_standard_layout_v<Payload>);
    LEARN_EXPECT_EQ(checks, migrations.size(), 4U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage14/section08/deprecated_in_cpp23", run>;

}  // namespace
