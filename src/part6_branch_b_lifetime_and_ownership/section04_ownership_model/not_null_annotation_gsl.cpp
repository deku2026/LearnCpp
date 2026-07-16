// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : not_null_annotation_gsl
// Topic id : part6/b/section04/not_null_annotation_gsl
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/b/section04/not_null_annotation_gsl";

template <class Pointer>
class NotNull {
public:
    explicit NotNull(Pointer pointer) : pointer_(pointer) {
        if (pointer_ == nullptr) throw std::invalid_argument{"NotNull received null"};
    }
    decltype(auto) operator*() const { return *pointer_; }
    Pointer get() const noexcept { return pointer_; }

private:
    Pointer pointer_;
};

static_assert(!std::default_initializable<NotNull<int*>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    int value = 23;
    const NotNull pointer{&value};
    LEARN_EXPECT_EQ(checks, *pointer, 23);
    LEARN_EXPECT(checks, pointer.get() == &value);
    LEARN_EXPECT_THROWS(checks, std::invalid_argument, NotNull<int*>{nullptr});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section04/not_null_annotation_gsl", run>;

}  // namespace
