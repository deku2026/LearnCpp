// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : sequence_associative_unordered_adapter_overview
// Topic id : part3/section02/sequence_associative_unordered_adapter_overview
// Reference: C++23 working draft [container.requirements], [container.adaptors].

#include "learn/example_support.hpp"

#include <array>
#include <deque>
#include <forward_list>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section02/sequence_associative_unordered_adapter_overview";
    learn::ExampleChecks checks{topic};

    // Sequence containers trade storage layout and iterator stability for different operations.
    std::array fixed{1, 2, 3};                   // fixed-size, contiguous
    std::vector<int> dynamic{1, 2};              // resizable, contiguous
    std::deque<int> double_ended{2, 3};          // efficient insertion at both ends
    std::list<int> linked{1, 4};                 // stable iterators; constant-time splice
    std::forward_list<int> singly_linked{2, 3};  // smallest node/link overhead

    dynamic.push_back(3);
    double_ended.push_front(1);
    std::list<int> donor{2, 3};
    linked.splice(std::next(linked.begin()), donor);
    singly_linked.push_front(1);
    LEARN_EXPECT_EQ(checks, fixed.front(), 1);
    LEARN_EXPECT_EQ(checks, dynamic, std::vector<int>({1, 2, 3}));
    LEARN_EXPECT_EQ(checks, double_ended.front(), 1);
    LEARN_EXPECT(checks, donor.empty());
    LEARN_EXPECT_EQ(checks, linked, std::list<int>({1, 2, 3, 4}));
    LEARN_EXPECT_EQ(checks, singly_linked.front(), 1);

    // Ordered associative containers expose key order (typically O(log N)); unordered containers
    // expose buckets (average O(1), worst-case O(N)) and make no iteration-order promise.
    std::map<std::string, int> ordered{{"beta", 2}, {"alpha", 1}};
    std::multiset<int> duplicates{2, 1, 2};
    std::unordered_map<std::string, int> hashed{{"alpha", 1}, {"beta", 2}};
    hashed.reserve(32);
    LEARN_EXPECT_EQ(checks, ordered.begin()->first, std::string{"alpha"});
    LEARN_EXPECT_EQ(checks, duplicates.count(2), std::size_t{2});
    LEARN_EXPECT_EQ(checks, hashed.at("beta"), 2);
    LEARN_EXPECT(checks, hashed.load_factor() <= hashed.max_load_factor());

    // Adaptors deliberately expose a restricted interface over an underlying sequence container.
    std::stack<int> lifo;
    std::queue<int> fifo;
    std::priority_queue<int> highest_first;
    for (const int value : {2, 1, 3}) {
        lifo.push(value);
        fifo.push(value);
        highest_first.push(value);
    }
    LEARN_EXPECT_EQ(checks, lifo.top(), 3);
    LEARN_EXPECT_EQ(checks, fifo.front(), 2);
    LEARN_EXPECT_EQ(checks, highest_first.top(), 3);

    // Choose by required complexity, reference/iterator invalidation, memory layout and ordering;
    // avoid defaulting to a node-based container merely because insertion itself is constant-time.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/sequence_associative_unordered_adapter_overview", run>;

}  // namespace
