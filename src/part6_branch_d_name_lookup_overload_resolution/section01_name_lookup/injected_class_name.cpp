// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : injected_class_name
// Topic id : part6/d/section01/injected_class_name
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part6/d/section01/injected_class_name";

template <class T>
struct Node {
    // Inside the class template, Node is the injected-class-name Node<T>.
    using self_type = Node;

    T value{};
    Node* next{};

    [[nodiscard]] constexpr Node& link(Node& other) noexcept {
        next = &other;
        return *this;
    }
};

static_assert(std::is_same_v<Node<int>::self_type, Node<int>>);
static_assert(std::is_same_v<decltype(std::declval<Node<int>>().next), Node<int>*>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Node<int> first{1};
    Node<int> second{2};
    Node<int>& returned = first.link(second);
    LEARN_EXPECT(checks, &returned == &first);
    LEARN_EXPECT(checks, first.next == &second);
    LEARN_EXPECT_EQ(checks, first.next->value, 2);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section01/injected_class_name", run>;

}  // namespace
