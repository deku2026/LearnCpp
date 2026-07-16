// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : owner_annotation_gsl
// Topic id : part6/b/section04/owner_annotation_gsl
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <memory>
#include <ranges>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part6/b/section04/owner_annotation_gsl";

template <class T>
class Owner {
public:
    explicit Owner(T* pointer) : pointer_(pointer) {}
    Owner(const Owner&) = delete;
    Owner(Owner&& other) noexcept : pointer_(std::exchange(other.pointer_, nullptr)) {}
    ~Owner() { delete pointer_; }
    T* get() const noexcept { return pointer_; }
    T* release() noexcept { return std::exchange(pointer_, nullptr); }

private:
    T* pointer_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Owner<int> owner{new int{23}};
    int* borrow = owner.get();
    LEARN_EXPECT_EQ(checks, *borrow, 23);
    std::unique_ptr<int> transferred{owner.release()};
    LEARN_EXPECT(checks, owner.get() == nullptr);
    LEARN_EXPECT_EQ(checks, *transferred, 23);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section04/owner_annotation_gsl", run>;

}  // namespace
