// A template-template parameter accepts a class template as a policy.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : template_template_parameter
// Topic id : part2/stage06/section01/template_template_parameter
//
// The same Stack definition is instantiated with vector and deque storage.

#include "learn/example_support.hpp"

#include <cstddef>
#include <deque>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

template <class T, template <class...> class Sequence>
class Stack {
public:
    void push(T value) { values_.push_back(std::move(value)); }
    [[nodiscard]] T pop() {
        T result = std::move(values_.back());
        values_.pop_back();
        return result;
    }
    [[nodiscard]] std::size_t size() const noexcept { return values_.size(); }

private:
    Sequence<T> values_;
};

static_assert(!std::is_same_v<Stack<int, std::vector>, Stack<int, std::deque>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/template_template_parameter"};
    Stack<int, std::vector> contiguous;
    Stack<int, std::deque> segmented;
    contiguous.push(3);
    segmented.push(8);
    LEARN_EXPECT_EQ(checks, contiguous.pop(), 3);
    LEARN_EXPECT_EQ(checks, segmented.pop(), 8);
    LEARN_EXPECT_EQ(checks, contiguous.size() + segmented.size(), std::size_t{0});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/template_template_parameter", run>;

}  // namespace
