// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : pool_resource
// Topic id : part6/c/section06/pool_resource
// Reference: C++23 working draft [mem.res.pool].

#include "learn/example_support.hpp"

#include <cstddef>
#include <list>
#include <memory_resource>

namespace {

class UpstreamTracker final : public std::pmr::memory_resource {
public:
    int allocations{};
    int deallocations{};
    std::size_t outstanding{};

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        void* result = std::pmr::new_delete_resource()->allocate(bytes, alignment);
        ++allocations;
        outstanding += bytes;
        return result;
    }

    void do_deallocate(void* pointer, std::size_t bytes, std::size_t alignment) override {
        ++deallocations;
        outstanding -= bytes;
        std::pmr::new_delete_resource()->deallocate(pointer, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override { return this == &other; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/pool_resource";
    learn::ExampleChecks checks{topic};
    UpstreamTracker upstream;
    {
        std::pmr::pool_options options{};
        options.max_blocks_per_chunk = 8;
        options.largest_required_pool_block = 128;
        std::pmr::unsynchronized_pool_resource pool{options, &upstream};

        {
            std::pmr::list<int> nodes{&pool};
            for (int value = 0; value < 32; ++value) {
                nodes.push_back(value);
            }
            LEARN_EXPECT_EQ(checks, nodes.size(), std::size_t{32});
            LEARN_EXPECT_EQ(checks, nodes.front(), 0);
            LEARN_EXPECT_EQ(checks, nodes.back(), 31);
            LEARN_EXPECT(checks, upstream.allocations >= 1);
        }

        // Freed blocks return to size-class pools for reuse. release discards
        // pooled allocations; an implementation may keep bookkeeping until the
        // resource itself is destroyed.
        const std::size_t before_release = upstream.outstanding;
        pool.release();
        LEARN_EXPECT(checks, upstream.outstanding <= before_release);
    }

    // Destruction returns all remaining upstream ownership. Use
    // synchronized_pool_resource when multiple threads share a pool.
    LEARN_EXPECT_EQ(checks, upstream.allocations, upstream.deallocations);
    LEARN_EXPECT_EQ(checks, upstream.outstanding, std::size_t{0});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/pool_resource", run>;

}  // namespace
