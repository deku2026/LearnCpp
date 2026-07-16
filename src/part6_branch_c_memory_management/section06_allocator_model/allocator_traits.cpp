// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_traits
// Topic id : part6/c/section06/allocator_traits
// Reference: C++23 working draft [allocator.traits].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>

namespace {

struct Counts {
    int allocations{};
    int deallocations{};
};

template <class T>
struct InstrumentedAllocator {
    using value_type = T;
    using propagate_on_container_move_assignment = std::true_type;

    Counts* counts{};

    explicit InstrumentedAllocator(Counts& observed) noexcept : counts(&observed) {}

    template <class U>
    InstrumentedAllocator(const InstrumentedAllocator<U>& other) noexcept : counts(other.counts) {}

    [[nodiscard]] T* allocate(std::size_t count) {
        ++counts->allocations;
        return std::allocator<T>{}.allocate(count);
    }

    void deallocate(T* pointer, std::size_t count) noexcept {
        ++counts->deallocations;
        std::allocator<T>{}.deallocate(pointer, count);
    }

    template <class U>
    friend struct InstrumentedAllocator;

    template <class U>
    friend bool operator==(const InstrumentedAllocator& left, const InstrumentedAllocator<U>& right) noexcept {
        return left.counts == right.counts;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/allocator_traits";
    learn::ExampleChecks checks{topic};
    Counts counts{};
    using Allocator = InstrumentedAllocator<std::string>;
    using Traits = std::allocator_traits<Allocator>;

    static_assert(std::is_same_v<Traits::pointer, std::string*>);
    static_assert(std::is_same_v<Traits::size_type, std::size_t>);
    static_assert(Traits::propagate_on_container_move_assignment::value);
    static_assert(std::is_same_v<Traits::rebind_alloc<int>, InstrumentedAllocator<int>>);

    Allocator allocator{counts};
    std::string* storage = Traits::allocate(allocator, 2);
    Traits::construct(allocator, storage, "alpha");
    Traits::construct(allocator, storage + 1, 3, 'x');
    LEARN_EXPECT_EQ(checks, storage[0], std::string{"alpha"});
    LEARN_EXPECT_EQ(checks, storage[1], std::string{"xxx"});
    Traits::destroy(allocator, storage + 1);
    Traits::destroy(allocator, storage);
    Traits::deallocate(allocator, storage, 2);
    LEARN_EXPECT_EQ(checks, counts.allocations, 1);
    LEARN_EXPECT_EQ(checks, counts.deallocations, 1);

    // allocator_traits supplies defaults for omitted aliases and construction hooks, letting generic
    // containers use minimal allocators, fancy pointers and propagation policies uniformly.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_traits", run>;

}  // namespace
