// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : pmr_memory_resource_cpp17
// Topic id : part3/section06/pmr_memory_resource_cpp17
// References: C++23 [mem.res], [mem.poly.allocator.class]

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory_resource>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section06/pmr_memory_resource_cpp17";

class TrackingResource final : public std::pmr::memory_resource {
public:
    explicit TrackingResource(std::pmr::memory_resource* upstream = std::pmr::new_delete_resource())
        : upstream_(upstream) {}

    [[nodiscard]] std::size_t allocations() const noexcept { return allocations_; }
    [[nodiscard]] std::size_t deallocations() const noexcept { return deallocations_; }
    [[nodiscard]] std::size_t bytes_outstanding() const noexcept { return bytes_outstanding_; }

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        void* pointer = upstream_->allocate(bytes, alignment);
        ++allocations_;
        bytes_outstanding_ += bytes;
        return pointer;
    }

    void do_deallocate(void* pointer, std::size_t bytes, std::size_t alignment) override {
        upstream_->deallocate(pointer, bytes, alignment);
        ++deallocations_;
        bytes_outstanding_ -= bytes;
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override { return this == &other; }

    std::pmr::memory_resource* upstream_;
    std::size_t allocations_{};
    std::size_t deallocations_{};
    std::size_t bytes_outstanding_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    TrackingResource first_resource;
    TrackingResource second_resource;

    using Vector = std::pmr::vector<int>;
    static_assert(std::is_same_v<Vector, std::vector<int, std::pmr::polymorphic_allocator<int>>>);
    {
        Vector first{&first_resource};
        Vector second{&second_resource};
        first.assign({2, 3, 5, 7, 11});
        second.assign({13, 17, 19});
        LEARN_EXPECT(checks, first.get_allocator().resource() == &first_resource);
        LEARN_EXPECT(checks, second.get_allocator().resource() == &second_resource);
        LEARN_EXPECT_EQ(checks, first.front() + first.back(), 13);

        std::pmr::vector<std::pmr::string> words{&first_resource};
        words.emplace_back(80, 'x');  // long enough to require string storage
        LEARN_EXPECT(checks, words.front().get_allocator().resource() == &first_resource);
        LEARN_EXPECT_EQ(checks, words.front().size(), std::size_t{80});
        LEARN_EXPECT(checks, first_resource.allocations() >= 2);
        LEARN_EXPECT(checks, first_resource.bytes_outstanding() > 0);
    }
    LEARN_EXPECT_EQ(checks, first_resource.allocations(), first_resource.deallocations());
    LEARN_EXPECT_EQ(checks, first_resource.bytes_outstanding(), std::size_t{0});
    LEARN_EXPECT_EQ(checks, second_resource.allocations(), second_resource.deallocations());

    std::pmr::polymorphic_allocator<int> erased{&first_resource};
    LEARN_EXPECT(checks, erased.resource() == &first_resource);
    LEARN_EXPECT(checks, first_resource.is_equal(first_resource));
    LEARN_EXPECT(checks, !first_resource.is_equal(second_resource));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/pmr_memory_resource_cpp17", run>;

}  // namespace
