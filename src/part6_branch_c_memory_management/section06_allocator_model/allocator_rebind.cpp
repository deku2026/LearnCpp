// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_rebind
// Topic id : part6/c/section06/allocator_rebind
// Reference: C++23 working draft [allocator.traits], rebind_alloc and rebind_traits.

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>

namespace {

template <class T>
struct TaggedAllocator {
    using value_type = T;
    int tag{};

    explicit TaggedAllocator(int value) noexcept : tag(value) {}

    template <class U>
    TaggedAllocator(const TaggedAllocator<U>& other) noexcept : tag(other.tag) {}

    [[nodiscard]] T* allocate(std::size_t count) { return std::allocator<T>{}.allocate(count); }
    void deallocate(T* pointer, std::size_t count) noexcept { std::allocator<T>{}.deallocate(pointer, count); }

    template <class U>
    friend struct TaggedAllocator;

    template <class U>
    friend bool operator==(const TaggedAllocator& left, const TaggedAllocator<U>& right) noexcept {
        return left.tag == right.tag;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/allocator_rebind";
    learn::ExampleChecks checks{topic};
    using IntAllocator = TaggedAllocator<int>;
    using StringAllocator = std::allocator_traits<IntAllocator>::rebind_alloc<std::string>;
    using StringTraits = std::allocator_traits<StringAllocator>;
    static_assert(std::is_same_v<StringAllocator, TaggedAllocator<std::string>>);

    IntAllocator integers{91};
    StringAllocator strings{integers};
    LEARN_EXPECT_EQ(checks, strings.tag, 91);
    std::string* storage = StringTraits::allocate(strings, 1);
    StringTraits::construct(strings, storage, "rebound");
    LEARN_EXPECT_EQ(checks, *storage, std::string{"rebound"});
    StringTraits::destroy(strings, storage);
    StringTraits::deallocate(strings, storage, 1);

    // Node-based containers rebind the user allocator to implementation node types. The converting
    // constructor must preserve the resource identity so any specialization can deallocate storage.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_rebind", run>;

}  // namespace
