// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : flat_containers_cpp23
// Topic id : part3/section02/flat_containers_cpp23
//
// Covers: std::flat_map / flat_set C++23 or sorted-vector fallback

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <version>
#if defined(__has_include)
#if __has_include(<flat_map>)
#include <flat_map>
#endif
#if __has_include(<flat_set>)
#include <flat_set>
#endif
#endif
namespace {

void demo_basics() {
    std::map<int, int> m{{1, 10}, {2, 20}};
    LEARN_CHECK(m[1] == 10);
}

void demo_intermediate() {
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L && __has_include(<flat_map>)
    std::flat_map<int, std::string> fm{{1, "a"}, {2, "b"}};
    LEARN_CHECK(fm[1] == "a");
    LEARN_CHECK(fm.size() == 2);
#else
    // flat map idea: sorted vector of pairs
    std::vector<std::pair<int, std::string>> fm{{1, "a"}, {2, "b"}};
    LEARN_CHECK(fm[0].second == "a");
#endif
}

void demo_expert() {
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L && __has_include(<flat_set>)
    std::flat_set<int> fs{3, 1, 2};
    LEARN_CHECK(fs.contains(2));
#else
    std::vector<int> fs{1, 2, 3};
    LEARN_CHECK(std::binary_search(fs.begin(), fs.end(), 2));
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/flat_containers_cpp23", run>;

}  // namespace
