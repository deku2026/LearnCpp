// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : heterogeneous_erase_extract_cpp23
// Topic id : part3/section02/heterogeneous_erase_extract_cpp23
//
// Covers: heterogeneous lookup erase/extract with transparent comparators

#include "learn/topic_registry.hpp"

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    std::map<std::string, int> m{{"alpha", 1}, {"beta", 2}};
    assert(m.find("alpha") != m.end());
    m.erase("beta");
    assert(m.size() == 1);
}

void demo_intermediate() {
    std::map<std::string, int, std::less<>> m{{"key", 42}};
    std::string_view sv = "key";
    auto it = m.find(sv);
    assert(it != m.end());
    assert(it->second == 42);
}

void demo_expert() {
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};
    auto node = m.extract("a");
    assert(node);
    assert(node.key() == "a");
    assert(m.size() == 1);
    m.insert(std::move(node));
    assert(m.size() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/heterogeneous_erase_extract_cpp23", run>;

}  // namespace
