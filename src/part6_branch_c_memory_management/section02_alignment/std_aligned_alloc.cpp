// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : std_aligned_alloc
// Topic id : part6/c/section02/std_aligned_alloc
// Reference: C++23 working draft [c.malloc]; Microsoft CRT uses _aligned_malloc instead.

#include "learn/example_support.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

#if defined(_WIN32)
#include <malloc.h>
#endif

namespace {

struct PlatformAlignedFree {
    void operator()(void* pointer) const noexcept {
#if defined(_WIN32)
        ::_aligned_free(pointer);
#else
        std::free(pointer);
#endif
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section02/std_aligned_alloc";
    learn::ExampleChecks checks{topic};
    constexpr std::size_t alignment = 64;
    constexpr std::size_t bytes = 2 * alignment;  // aligned_alloc requires a size multiple.

#if defined(_WIN32)
    // The Microsoft CRT does not provide std::aligned_alloc because std::free cannot release its
    // over-aligned blocks; use the explicitly paired platform functions instead.
    std::unique_ptr<void, PlatformAlignedFree> storage{::_aligned_malloc(bytes, alignment)};
#else
    std::unique_ptr<void, PlatformAlignedFree> storage{std::aligned_alloc(alignment, bytes)};
#endif
    LEARN_EXPECT(checks, storage != nullptr);
    if (storage != nullptr) {
        LEARN_EXPECT_EQ(checks, reinterpret_cast<std::uintptr_t>(storage.get()) % alignment, std::uintptr_t{0});
    }

#if 0
    void* invalid_size = std::aligned_alloc(64, 65);  // Size is not a multiple of alignment.
    // A failed call may return null; passing it to typed code without checking is invalid.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/std_aligned_alloc", run>;

}  // namespace
