// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : is_always_equal
// Topic id : part6/c/section06/is_always_equal
// Reference: C++23 working draft [allocator.traits], [allocator.requirements].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <type_traits>

namespace {

template <class T>
struct StatelessAllocator {
    using value_type = T;
    using is_always_equal = std::true_type;

    [[nodiscard]] T* allocate(std::size_t count) { return std::allocator<T>{}.allocate(count); }
    void deallocate(T* pointer, std::size_t count) noexcept { std::allocator<T>{}.deallocate(pointer, count); }
};

template <class T, class U>
bool operator==(const StatelessAllocator<T>&, const StatelessAllocator<U>&) noexcept {
    return true;
}

template <class T>
struct ResourceAllocator {
    using value_type = T;
    using is_always_equal = std::false_type;

    int* resource{};

    template <class U>
    ResourceAllocator(const ResourceAllocator<U>& other) noexcept : resource(other.resource) {}
    explicit ResourceAllocator(int& id) noexcept : resource(&id) {}

    [[nodiscard]] T* allocate(std::size_t count) { return std::allocator<T>{}.allocate(count); }
    void deallocate(T* pointer, std::size_t count) noexcept { std::allocator<T>{}.deallocate(pointer, count); }

    template <class U>
    friend struct ResourceAllocator;

    template <class U>
    friend bool operator==(const ResourceAllocator& left, const ResourceAllocator<U>& right) noexcept {
        return left.resource == right.resource;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/is_always_equal";
    learn::ExampleChecks checks{topic};
    using StatelessTraits = std::allocator_traits<StatelessAllocator<int>>;
    using StatefulTraits = std::allocator_traits<ResourceAllocator<int>>;
    static_assert(StatelessTraits::is_always_equal::value);
    static_assert(!StatefulTraits::is_always_equal::value);

    StatelessAllocator<int> first;
    StatelessAllocator<long> second;
    LEARN_EXPECT(checks, first == second);
    int resource_a = 1;
    int resource_b = 2;
    ResourceAllocator<int> a{resource_a};
    ResourceAllocator<int> same_a{resource_a};
    ResourceAllocator<int> b{resource_b};
    LEARN_EXPECT(checks, a == same_a);
    LEARN_EXPECT(checks, !(a == b));

    // is_always_equal promises that storage from any allocator instance can be released by any
    // other instance of that type. A false value forces containers to inspect runtime equality.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/is_always_equal", run>;

}  // namespace
