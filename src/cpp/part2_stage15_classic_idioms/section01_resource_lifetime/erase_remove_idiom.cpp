// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §1.6
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : erase_remove_idiom
// Topic id : part2/stage15/section01/erase_remove_idiom
//
// Erase-remove: the classic way to delete elements from a sequence container.
// Motive : remove_if only partitions; erase shrinks the container.
// Modern : C++20 std::erase / std::erase_if one-liners (prefer these).
// Pitfall: forgetting erase leaves "moved-from garbage" at the end; O(n) shifts.

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

void print_vec(std::string_view label, const std::vector<int>& v) {
    std::cout << "  " << label << ": [";
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) {
            std::cout << ", ";
        }
        std::cout << v[i];
    }
    std::cout << "]\n";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== classic erase-remove (pre-C++20) ==\n";
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    print_vec("before", v);

    // remove_if moves keepers forward; returns new logical end (does NOT resize).
    auto logical_end = std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    std::cout << "  after remove_if only: size still " << v.size() << " (tail is unspecified junk conceptually)\n";

    v.erase(logical_end, v.end());  // actually drop the tail
    print_vec("after erase", v);
    assert((v == std::vector<int>{1, 3, 5, 7}));

    std::cout << "== value erase-remove ==\n";
    std::vector<int> w{1, 2, 2, 3, 2, 4};
    w.erase(std::remove(w.begin(), w.end(), 2), w.end());
    assert((w == std::vector<int>{1, 3, 4}));
    print_vec("removed 2s", w);

    std::cout << "== C++20 std::erase_if (preferred) ==\n";
    std::vector<int> u{10, 15, 20, 25, 30};
    const std::size_t removed = std::erase_if(u, [](int x) { return x % 10 == 0; });
    assert(removed == 3);
    std::cout << "  erase_if removed " << removed << " elements\n";
    assert((u == std::vector<int>{15, 25}));
    print_vec("erase_if tens", u);

    std::cout << "== C++20 std::erase by value ==\n";
    std::vector<std::string> names{"a", "b", "a", "c"};
    const auto n = std::erase(names, "a");
    assert(n == 2);
    assert((names == std::vector<std::string>{"b", "c"}));
    std::cout << "  erased " << n << " \"a\"s\n";

    std::cout << "note: list/map use their own erase; erase-remove is for contiguous sequences\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/erase_remove_idiom", run>;

}  // namespace
