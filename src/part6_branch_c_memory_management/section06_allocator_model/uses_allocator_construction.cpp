// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : uses_allocator_construction
// Topic id : part6/c/section06/uses_allocator_construction
// Reference: C++23 working draft [allocator.uses], [allocator.adaptor].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <memory_resource>
#include <scoped_allocator>
#include <type_traits>
#include <vector>

namespace {

struct AllocatorAwareValue {
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

    int value{};
    std::pmr::memory_resource* resource{};

    AllocatorAwareValue(std::allocator_arg_t, allocator_type allocator, int initial)
        : value(initial), resource(allocator.resource()) {}
};

template <class T>
struct IdAllocator {
    using value_type = T;
    int id{};

    explicit IdAllocator(int value) noexcept : id(value) {}
    template <class U>
    IdAllocator(const IdAllocator<U>& other) noexcept : id(other.id) {}

    [[nodiscard]] T* allocate(std::size_t count) { return std::allocator<T>{}.allocate(count); }
    void deallocate(T* pointer, std::size_t count) noexcept { std::allocator<T>{}.deallocate(pointer, count); }

    template <class U>
    friend struct IdAllocator;

    template <class U>
    friend bool operator==(const IdAllocator& left, const IdAllocator<U>& right) noexcept {
        return left.id == right.id;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/uses_allocator_construction";
    learn::ExampleChecks checks{topic};
    std::pmr::monotonic_buffer_resource resource;
    std::pmr::polymorphic_allocator<std::byte> allocator{&resource};
    static_assert(std::uses_allocator_v<AllocatorAwareValue, decltype(allocator)>);

    auto value = std::make_obj_using_allocator<AllocatorAwareValue>(allocator, 42);
    LEARN_EXPECT_EQ(checks, value.value, 42);
    LEARN_EXPECT_EQ(checks, value.resource, &resource);

    alignas(AllocatorAwareValue) std::byte storage[sizeof(AllocatorAwareValue)];
    auto* placed =
        std::uninitialized_construct_using_allocator(reinterpret_cast<AllocatorAwareValue*>(storage), allocator, 73);
    LEARN_EXPECT_EQ(checks, placed->value, 73);
    LEARN_EXPECT_EQ(checks, placed->resource, &resource);
    std::destroy_at(placed);

    // scoped_allocator_adaptor forwards an inner allocator recursively to nested containers.
    using Inner = std::vector<int, IdAllocator<int>>;
    using OuterAllocator = IdAllocator<Inner>;
    using ScopedAllocator = std::scoped_allocator_adaptor<OuterAllocator, IdAllocator<int>>;
    ScopedAllocator scoped{OuterAllocator{1}, IdAllocator<int>{77}};
    std::vector<Inner, ScopedAllocator> nested{scoped};
    nested.emplace_back();
    nested.back().push_back(5);
    LEARN_EXPECT_EQ(checks, nested.back().get_allocator().id, 77);
    LEARN_EXPECT_EQ(checks, nested.back().front(), 5);

    // uses-allocator construction selects allocator_arg-leading or allocator-trailing constructors;
    // declaring allocator_type without a compatible constructor makes construction ill-formed.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/uses_allocator_construction", run>;

}  // namespace
