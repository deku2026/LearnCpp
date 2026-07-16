// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : new_delete_global
// Topic id : part6/c/section01/new_delete_global
// Reference: C++23 working draft [new.delete], [expr.new], [expr.delete].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <new>

namespace {

struct Payload {
    int value;
};

struct RawStorageDelete {
    void operator()(void* pointer) const noexcept { ::operator delete(pointer); }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section01/new_delete_global";
    learn::ExampleChecks checks{topic};

    // A new-expression obtains storage and then constructs an object; delete destroys first and
    // releases storage second. unique_ptr makes the pairing exception-safe.
    auto owner = std::make_unique<Payload>(Payload{42});
    LEARN_EXPECT_EQ(checks, owner->value, 42);
    owner.reset();
    LEARN_EXPECT(checks, owner == nullptr);

    // Direct allocation functions return raw storage only. Construction and destruction are
    // explicit, and the same allocation/deallocation family must be paired.
    std::unique_ptr<void, RawStorageDelete> storage{::operator new(sizeof(Payload))};
    auto* object = std::construct_at(static_cast<Payload*>(storage.get()), Payload{17});
    LEARN_EXPECT_EQ(checks, object->value, 17);
    std::destroy_at(object);

    // Nothrow new reports allocation failure with nullptr. Never dereference before checking.
    std::unique_ptr<int[]> array{new (std::nothrow) int[4]{2, 3, 5, 7}};
    LEARN_EXPECT(checks, array != nullptr);
    if (array != nullptr) {
        LEARN_EXPECT_EQ(checks, array[0] + array[1] + array[2] + array[3], 17);
    }

#if 0
    auto* wrong = new int[4];
    delete wrong;  // Undefined behavior: new[] must be paired with delete[].
    int* twice = new int{1};
    delete twice;
    delete twice;  // Undefined behavior: double deletion.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/new_delete_global", run>;

}  // namespace
