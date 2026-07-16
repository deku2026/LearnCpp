// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : swap_two_step_idiom
// Topic id : part6/d/section04/swap_two_step_idiom
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part6/d/section04/swap_two_step_idiom";

namespace resource {

struct Buffer {
    std::string contents;
    int* adl_calls{};

    friend void swap(Buffer& left, Buffer& right) noexcept {
        using std::swap;
        swap(left.contents, right.contents);
        swap(left.adl_calls, right.adl_calls);
        if (left.adl_calls != nullptr) {
            ++*left.adl_calls;
        }
    }
};

}  // namespace resource

template <class T>
void exchange(T& left, T& right) noexcept(std::is_nothrow_swappable_v<T>) {
    using std::swap;    // fallback
    swap(left, right);  // unqualified call enables ADL
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int calls{};
    resource::Buffer left{"alpha", &calls};
    resource::Buffer right{"beta", &calls};
    exchange(left, right);
    LEARN_EXPECT_EQ(checks, left.contents, std::string{"beta"});
    LEARN_EXPECT_EQ(checks, calls, 1);

    int first = 1;
    int second = 2;
    exchange(first, second);  // no ADL customization; std::swap is the fallback
    LEARN_EXPECT_EQ(checks, first, 2);
    LEARN_EXPECT_EQ(checks, second, 1);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section04/swap_two_step_idiom", run>;

}  // namespace
