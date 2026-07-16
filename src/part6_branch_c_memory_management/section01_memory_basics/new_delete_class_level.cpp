// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : new_delete_class_level
// Topic id : part6/c/section01/new_delete_class_level
// Reference: C++23 working draft [class.free], [expr.new], [expr.delete].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <new>

namespace {

struct ClassAllocated {
    static inline int allocations{};
    static inline int deallocations{};

    int value{};

    static void* operator new(std::size_t bytes) {
        ++allocations;
        return ::operator new(bytes);
    }

    static void operator delete(void* pointer) noexcept {
        ++deallocations;
        ::operator delete(pointer);
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section01/new_delete_class_level";
    learn::ExampleChecks checks{topic};
    ClassAllocated::allocations = 0;
    ClassAllocated::deallocations = 0;

    {
        std::unique_ptr<ClassAllocated> owner{new ClassAllocated{73}};
        LEARN_EXPECT_EQ(checks, owner->value, 73);
        LEARN_EXPECT_EQ(checks, ClassAllocated::allocations, 1);
        LEARN_EXPECT_EQ(checks, ClassAllocated::deallocations, 0);
    }
    LEARN_EXPECT_EQ(checks, ClassAllocated::allocations, 1);
    LEARN_EXPECT_EQ(checks, ClassAllocated::deallocations, 1);

    // Class lookup selects these functions only for ClassAllocated scalar new/delete expressions.
    // They still delegate to the global functions, preserving the required pairing and alignment.
    auto ordinary = std::make_unique<int>(9);
    LEARN_EXPECT_EQ(checks, *ordinary, 9);
    LEARN_EXPECT_EQ(checks, ClassAllocated::allocations, 1);

    // If construction throws after class-specific allocation, the matching class-specific delete
    // is invoked automatically. Production overload sets must also consider aligned, sized,
    // nothrow and array forms instead of accidentally hiding a form that callers require.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/new_delete_class_level", run>;

}  // namespace
