// Default template arguments provide a common policy with an explicit override.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : default_template_arguments
// Topic id : part2/stage06/section01/default_template_arguments
//
// Bag<int> uses vector<int>; callers may select another compatible container.

#include "learn/example_support.hpp"

#include <cstddef>
#include <deque>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

template <class T, class Container = std::vector<T>>
class Bag {
public:
    using container_type = Container;

    void add(T value) { values_.push_back(std::move(value)); }
    [[nodiscard]] std::size_t size() const noexcept { return values_.size(); }
    [[nodiscard]] const T& front() const { return values_.front(); }

private:
    Container values_;
};

static_assert(std::is_same_v<Bag<int>::container_type, std::vector<int>>);
static_assert(std::is_same_v<Bag<int, std::deque<int>>::container_type, std::deque<int>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/default_template_arguments"};
    Bag<int> default_bag;
    default_bag.add(4);
    Bag<int, std::deque<int>> deque_bag;
    deque_bag.add(7);
    LEARN_EXPECT_EQ(checks, default_bag.front(), 4);
    LEARN_EXPECT_EQ(checks, deque_bag.front(), 7);
    LEARN_EXPECT_EQ(checks, default_bag.size() + deque_bag.size(), std::size_t{2});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/default_template_arguments", run>;

}  // namespace
