// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section03_custom_allocators_and_pools
// Item     : linear_arena_allocator
// Topic id : part6/c/section03/linear_arena_allocator
// Reference: C++23 working draft [ptr.align], [specialized.construct].

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <new>
#include <string>
#include <utility>

namespace {

template <std::size_t Capacity>
class LinearArena {
public:
    [[nodiscard]] void* allocate(std::size_t bytes, std::size_t alignment) {
        void* candidate = storage_.data() + offset_;
        std::size_t remaining = storage_.size() - offset_;
        void* aligned = std::align(alignment, bytes, candidate, remaining);
        if (aligned == nullptr) {
            throw std::bad_alloc{};
        }
        offset_ = static_cast<std::byte*>(aligned) - storage_.data() + bytes;
        return aligned;
    }

    template <class T, class... Args>
    [[nodiscard]] T* create(Args&&... args) {
        void* storage = allocate(sizeof(T), alignof(T));
        return std::construct_at(static_cast<T*>(storage), std::forward<Args>(args)...);
    }

    [[nodiscard]] std::size_t used() const noexcept { return offset_; }
    [[nodiscard]] constexpr std::size_t capacity() const noexcept { return Capacity; }

    void reset() noexcept { offset_ = 0; }

private:
    alignas(std::max_align_t) std::array<std::byte, Capacity> storage_{};
    std::size_t offset_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section03/linear_arena_allocator";
    learn::ExampleChecks checks{topic};
    LinearArena<128> arena;

    int* number = arena.create<int>(42);
    std::string* text = arena.create<std::string>("arena-owned storage");
    LEARN_EXPECT_EQ(checks, *number, 42);
    LEARN_EXPECT_EQ(checks, *text, std::string{"arena-owned storage"});
    LEARN_EXPECT(checks, arena.used() <= arena.capacity());

    std::destroy_at(text);
    std::destroy_at(number);
    arena.reset();  // Only valid after every object using the arena has ended its lifetime.
    LEARN_EXPECT_EQ(checks, arena.used(), std::size_t{0});

    LEARN_EXPECT_THROWS(checks, std::bad_alloc, arena.allocate(arena.capacity() + 1, 1));
    // Individual deallocation is intentionally absent: allocation is O(1), and the whole arena is
    // reclaimed together. A throwing constructor may consume space but cannot leak an object.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section03/linear_arena_allocator", run>;

}  // namespace
