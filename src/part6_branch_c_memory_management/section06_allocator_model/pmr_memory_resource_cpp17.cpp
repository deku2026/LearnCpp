// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : pmr_memory_resource_cpp17
// Topic id : part6/c/section06/pmr_memory_resource_cpp17
// Reference: C++23 working draft [mem.res.class], [mem.poly.allocator.class].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory_resource>
#include <string>
#include <vector>

namespace {

class CountingResource final : public std::pmr::memory_resource {
public:
    explicit CountingResource(std::pmr::memory_resource* upstream) : upstream_(upstream) {}

    int allocation_calls{};
    int deallocation_calls{};
    std::size_t outstanding_bytes{};

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        void* result = upstream_->allocate(bytes, alignment);
        ++allocation_calls;
        outstanding_bytes += bytes;
        return result;
    }

    void do_deallocate(void* pointer, std::size_t bytes, std::size_t alignment) override {
        ++deallocation_calls;
        outstanding_bytes -= bytes;
        upstream_->deallocate(pointer, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override { return this == &other; }

    std::pmr::memory_resource* upstream_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/pmr_memory_resource_cpp17";
    learn::ExampleChecks checks{topic};
    CountingResource resource{std::pmr::new_delete_resource()};

    {
        std::pmr::vector<int> values{std::pmr::polymorphic_allocator<int>{&resource}};
        values.assign({2, 3, 5, 7});
        std::pmr::string text{"polymorphic", &resource};
        LEARN_EXPECT_EQ(checks, values.back(), 7);
        LEARN_EXPECT_EQ(checks, text, (std::pmr::string{"polymorphic", &resource}));
        LEARN_EXPECT(checks, resource.allocation_calls >= 1);
        LEARN_EXPECT(checks, resource.outstanding_bytes > 0);
    }

    LEARN_EXPECT_EQ(checks, resource.allocation_calls, resource.deallocation_calls);
    LEARN_EXPECT_EQ(checks, resource.outstanding_bytes, std::size_t{0});
    // memory_resource gives runtime polymorphism while polymorphic_allocator remains a small value.
    // The resource must outlive every allocation made through it; equality controls interchangeability.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/pmr_memory_resource_cpp17", run>;

}  // namespace
