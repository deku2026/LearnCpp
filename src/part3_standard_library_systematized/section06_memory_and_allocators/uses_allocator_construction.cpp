// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : uses_allocator_construction
// Topic id : part3/section06/uses_allocator_construction
// References: C++23 [allocator.uses.construction], [allocator.uses.trait]

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <memory_resource>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part3/section06/uses_allocator_construction";

struct LeadingAllocatorValue {
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

    LeadingAllocatorValue(std::allocator_arg_t, allocator_type allocator, int initial)
        : value(initial), resource(allocator.resource()) {}

    int value{};
    std::pmr::memory_resource* resource{};
};

struct TrailingAllocatorValue {
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

    TrailingAllocatorValue(int initial, allocator_type allocator) : value(initial), resource(allocator.resource()) {}

    int value{};
    std::pmr::memory_resource* resource{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::pmr::monotonic_buffer_resource resource;
    std::pmr::polymorphic_allocator<std::byte> allocator{&resource};

    static_assert(std::uses_allocator_v<LeadingAllocatorValue, decltype(allocator)>);
    static_assert(std::uses_allocator_v<TrailingAllocatorValue, decltype(allocator)>);

    auto leading = std::make_obj_using_allocator<LeadingAllocatorValue>(allocator, 42);
    auto trailing = std::make_obj_using_allocator<TrailingAllocatorValue>(allocator, 84);
    LEARN_EXPECT_EQ(checks, leading.value, 42);
    LEARN_EXPECT(checks, leading.resource == &resource);
    LEARN_EXPECT_EQ(checks, trailing.value, 84);
    LEARN_EXPECT(checks, trailing.resource == &resource);

    // The tuple exposes which of the two standard constructor conventions was
    // selected: allocator_arg first, or allocator last.
    auto leading_arguments = std::uses_allocator_construction_args<LeadingAllocatorValue>(allocator, 7);
    auto trailing_arguments = std::uses_allocator_construction_args<TrailingAllocatorValue>(allocator, 9);
    static_assert(std::tuple_size_v<decltype(leading_arguments)> == 3);
    static_assert(std::tuple_size_v<decltype(trailing_arguments)> == 2);

    alignas(LeadingAllocatorValue) std::array<std::byte, sizeof(LeadingAllocatorValue)> storage{};
    auto* location = reinterpret_cast<LeadingAllocatorValue*>(storage.data());
    LeadingAllocatorValue* constructed = std::uninitialized_construct_using_allocator(location, allocator, 123);
    LEARN_EXPECT_EQ(checks, constructed->value, 123);
    LEARN_EXPECT(checks, constructed->resource == &resource);
    std::destroy_at(constructed);

    // std::uses_allocator only detects allocator_type convertibility; the
    // construction helpers additionally enforce a valid leading/trailing ctor.
    static_assert(!std::uses_allocator_v<int, decltype(allocator)>);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/uses_allocator_construction", run>;

}  // namespace
