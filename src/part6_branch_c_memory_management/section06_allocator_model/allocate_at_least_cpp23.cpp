// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocate_at_least_cpp23
// Topic id : part6/c/section06/allocate_at_least_cpp23
// Reference: WG21 P0401R6 (__cpp_lib_allocate_at_least).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <cstddef>
#include <memory>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/allocate_at_least_cpp23";
    learn::ExampleChecks checks{topic};
    std::allocator<int> allocator;
    using Traits = std::allocator_traits<decltype(allocator)>;
    constexpr std::size_t requested = 3;

#if defined(__cpp_lib_allocate_at_least) && __cpp_lib_allocate_at_least >= 202302L
    auto allocation = Traits::allocate_at_least(allocator, requested);
    LEARN_EXPECT(checks, allocation.ptr != nullptr);
    LEARN_EXPECT(checks, allocation.count >= requested);
    for (std::size_t index = 0; index < requested; ++index) {
        Traits::construct(allocator, allocation.ptr + index, static_cast<int>(index + 1));
    }
    LEARN_EXPECT_EQ(checks, allocation.ptr[0] + allocation.ptr[1] + allocation.ptr[2], 6);
    for (std::size_t index = requested; index != 0; --index) {
        Traits::destroy(allocator, allocation.ptr + index - 1);
    }
    Traits::deallocate(allocator, allocation.ptr, allocation.count);
    // The returned count, not the requested count, must be supplied when deallocating.
    return checks.result();
#else
    int* storage = Traits::allocate(allocator, requested);
    for (std::size_t index = 0; index < requested; ++index) {
        Traits::construct(allocator, storage + index, static_cast<int>(index + 1));
    }
    LEARN_EXPECT_EQ(checks, storage[0] + storage[1] + storage[2], 6);
    for (std::size_t index = requested; index != 0; --index) {
        Traits::destroy(allocator, storage + index - 1);
    }
    Traits::deallocate(allocator, storage, requested);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_allocate_at_least >= 202302L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocate_at_least_cpp23", run>;

}  // namespace
