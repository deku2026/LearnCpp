// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : pool_resource_cpp17
// Topic id : part3/section06/pool_resource_cpp17
// References: C++23 [mem.res.pool], [mem.res.pool.options]

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <memory_resource>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section06/pool_resource_cpp17";

class CountingResource final : public std::pmr::memory_resource {
public:
    [[nodiscard]] std::size_t allocations() const noexcept { return allocations_; }
    [[nodiscard]] std::size_t deallocations() const noexcept { return deallocations_; }
    [[nodiscard]] std::size_t outstanding() const noexcept { return outstanding_; }

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        void* pointer = std::pmr::new_delete_resource()->allocate(bytes, alignment);
        ++allocations_;
        outstanding_ += bytes;
        return pointer;
    }

    void do_deallocate(void* pointer, std::size_t bytes, std::size_t alignment) override {
        std::pmr::new_delete_resource()->deallocate(pointer, bytes, alignment);
        ++deallocations_;
        outstanding_ -= bytes;
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override { return this == &other; }

    std::size_t allocations_{};
    std::size_t deallocations_{};
    std::size_t outstanding_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    CountingResource upstream;
    {
        std::pmr::pool_options options;
        options.max_blocks_per_chunk = 8;
        options.largest_required_pool_block = 128;
        std::pmr::unsynchronized_pool_resource pool{options, &upstream};

        std::array<void*, 12> blocks{};
        for (void*& block : blocks) {
            block = pool.allocate(32, alignof(std::max_align_t));
            LEARN_EXPECT(checks, block != nullptr);
        }
        LEARN_EXPECT(checks, upstream.allocations() > 0);
        for (void* block : blocks) {
            pool.deallocate(block, 32, alignof(std::max_align_t));
        }

        // Small deallocations return blocks to the pool; chunks remain cached.
        LEARN_EXPECT(checks, upstream.outstanding() > 0);
        {
            std::pmr::vector<int> values{&pool};
            values.assign(50, 7);
            LEARN_EXPECT_EQ(checks, values.size(), std::size_t{50});
            LEARN_EXPECT_EQ(checks, values.front(), 7);
        }

        const std::size_t before_release = upstream.outstanding();
        pool.release();
        // Implementations may retain pool bookkeeping until destruction, but
        // release must discard pooled allocations and never increase ownership.
        LEARN_EXPECT(checks, upstream.outstanding() <= before_release);
    }

    // Destruction releases any implementation bookkeeping obtained from the
    // explicitly supplied upstream resource.
    LEARN_EXPECT_EQ(checks, upstream.outstanding(), std::size_t{0});
    LEARN_EXPECT_EQ(checks, upstream.allocations(), upstream.deallocations());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/pool_resource_cpp17", run>;

}  // namespace
