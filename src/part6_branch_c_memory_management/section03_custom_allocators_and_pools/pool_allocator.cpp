// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section03_custom_allocators_and_pools
// Item     : pool_allocator
// Topic id : part6/c/section03/pool_allocator
// Reference: C++23 working draft [basic.life], [specialized.construct].

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <new>
#include <string>
#include <utility>

namespace {

template <class T, std::size_t Capacity>
class FixedPool {
public:
    template <class... Args>
    [[nodiscard]] T* create(Args&&... args) {
        const std::size_t index = acquire_slot();
        T* pointer = reinterpret_cast<T*>(slots_[index].bytes);
        try {
            return std::construct_at(pointer, std::forward<Args>(args)...);
        } catch (...) {
            used_[index] = false;
            throw;
        }
    }

    void destroy(T* pointer) noexcept {
        std::destroy_at(pointer);
        for (std::size_t index = 0; index < Capacity; ++index) {
            if (static_cast<void*>(slots_[index].bytes) == static_cast<void*>(pointer)) {
                used_[index] = false;
                return;
            }
        }
    }

    [[nodiscard]] std::size_t active() const noexcept {
        std::size_t count{};
        for (const bool occupied : used_) {
            count += occupied;
        }
        return count;
    }

private:
    struct Slot {
        alignas(T) std::byte bytes[sizeof(T)];
    };

    [[nodiscard]] std::size_t acquire_slot() {
        for (std::size_t index = 0; index < Capacity; ++index) {
            if (!used_[index]) {
                used_[index] = true;
                return index;
            }
        }
        throw std::bad_alloc{};
    }

    std::array<Slot, Capacity> slots_{};
    std::array<bool, Capacity> used_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section03/pool_allocator";
    learn::ExampleChecks checks{topic};
    FixedPool<std::string, 2> pool;

    std::string* first = pool.create("first");
    std::string* second = pool.create("second");
    LEARN_EXPECT_EQ(checks, pool.active(), std::size_t{2});
    LEARN_EXPECT_THROWS(checks, std::bad_alloc, pool.create("full"));

    pool.destroy(first);
    std::string* reused = pool.create("reused");
    LEARN_EXPECT_EQ(checks, reused, first);
    LEARN_EXPECT_EQ(checks, *reused, std::string{"reused"});
    pool.destroy(second);
    pool.destroy(reused);
    LEARN_EXPECT_EQ(checks, pool.active(), std::size_t{0});

#if 0
    pool.destroy(reused);  // Undefined behavior: the object's lifetime already ended.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section03/pool_allocator", run>;

}  // namespace
