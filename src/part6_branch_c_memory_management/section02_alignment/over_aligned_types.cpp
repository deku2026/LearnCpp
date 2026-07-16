// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : over_aligned_types
// Topic id : part6/c/section02/over_aligned_types
// Reference: C++23 working draft [new.delete.single], [expr.new], [basic.align].

#include "learn/example_support.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <new>

namespace {

struct alignas(64) CacheBlock {
    int value{};
};

struct RawAlignedDelete {
    void operator()(void* pointer) const noexcept { ::operator delete(pointer, std::align_val_t{alignof(CacheBlock)}); }
};

struct CacheBlockDelete {
    void operator()(CacheBlock* pointer) const noexcept {
        if (pointer != nullptr) {
            std::destroy_at(pointer);
            ::operator delete(pointer, std::align_val_t{alignof(CacheBlock)});
        }
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section02/over_aligned_types";
    learn::ExampleChecks checks{topic};
    static_assert(alignof(CacheBlock) > __STDCPP_DEFAULT_NEW_ALIGNMENT__);

    // Since C++17, an ordinary new-expression selects an aligned allocation overload when needed.
    auto automatic_pairing = std::make_unique<CacheBlock>(CacheBlock{11});
    LEARN_EXPECT_EQ(checks, reinterpret_cast<std::uintptr_t>(automatic_pairing.get()) % alignof(CacheBlock),
                    std::uintptr_t{0});

    // A direct aligned allocation call must be paired with the matching aligned delete overload.
    std::unique_ptr<void, RawAlignedDelete> raw{
        ::operator new(sizeof(CacheBlock), std::align_val_t{alignof(CacheBlock)})};
    CacheBlock* object = std::construct_at(static_cast<CacheBlock*>(raw.get()), CacheBlock{29});
    static_cast<void>(raw.release());
    std::unique_ptr<CacheBlock, CacheBlockDelete> owner{object};
    LEARN_EXPECT_EQ(checks, owner->value, 29);
    LEARN_EXPECT_EQ(checks, reinterpret_cast<std::uintptr_t>(owner.get()) % alignof(CacheBlock), std::uintptr_t{0});

#if 0
    void* mismatched = ::operator new(sizeof(CacheBlock), std::align_val_t{64});
    ::operator delete(mismatched);  // Undefined behavior: missing the aligned delete argument.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/over_aligned_types", run>;

}  // namespace
