// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : monotonic_buffer_resource_cpp17
// Topic id : part3/section06/monotonic_buffer_resource_cpp17
// References: C++23 [mem.res.monotonic.buffer]

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <memory_resource>
#include <new>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section06/monotonic_buffer_resource_cpp17";

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

    alignas(std::max_align_t) std::array<std::byte, 256> local_buffer{};
    CountingResource upstream;
    std::pmr::monotonic_buffer_resource arena{local_buffer.data(), local_buffer.size(), &upstream};
    void* first = arena.allocate(32, alignof(std::max_align_t));
    void* second = arena.allocate(48, alignof(std::max_align_t));
    LEARN_EXPECT(checks, first != nullptr && second != nullptr && first != second);
    LEARN_EXPECT_EQ(checks, upstream.allocations(), std::size_t{0});

    // Deallocate is intentionally a no-op; arena storage is reclaimed in bulk.
    arena.deallocate(first, 32, alignof(std::max_align_t));
    LEARN_EXPECT_EQ(checks, upstream.deallocations(), std::size_t{0});
    static_cast<void>(arena.allocate(1024, alignof(std::max_align_t)));
    LEARN_EXPECT(checks, upstream.allocations() > 0);
    LEARN_EXPECT(checks, upstream.outstanding() > 0);
    arena.release();
    LEARN_EXPECT_EQ(checks, upstream.outstanding(), std::size_t{0});
    LEARN_EXPECT_EQ(checks, upstream.allocations(), upstream.deallocations());

    // null_memory_resource turns exhausting a fixed-capacity arena into a
    // deterministic exception instead of silently using the heap.
    alignas(std::max_align_t) std::array<std::byte, 64> bounded_buffer{};
    std::pmr::monotonic_buffer_resource bounded{bounded_buffer.data(), bounded_buffer.size(),
                                                std::pmr::null_memory_resource()};
    static_cast<void>(bounded.allocate(16));
    LEARN_EXPECT_THROWS(checks, std::bad_alloc, bounded.allocate(1024));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/monotonic_buffer_resource_cpp17", run>;

}  // namespace
