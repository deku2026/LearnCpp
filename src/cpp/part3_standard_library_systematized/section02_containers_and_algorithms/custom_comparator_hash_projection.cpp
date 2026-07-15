// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : custom_comparator_hash_projection
// Topic id : part3/section02/custom_comparator_hash_projection
//
// Covers: custom comparator, hash, and ranges projections

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <functional>
#include <ranges>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

void demo_basics() {
    std::set<int, std::greater<int>> s{1, 2, 3};
    assert(*s.begin() == 3);
}

void demo_intermediate() {
    struct CaseInsensitive {
        bool operator()(const std::string& a, const std::string& b) const {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), [](char x, char y) {
                return std::tolower(static_cast<unsigned char>(x)) < std::tolower(static_cast<unsigned char>(y));
            });
        }
    };
    std::set<std::string, CaseInsensitive> s{"B", "a"};
    assert(s.size() == 2);
}

void demo_expert() {
    struct Person {
        std::string name;
        int age;
    };
    std::vector<Person> people{{"bob", 30}, {"ann", 20}};
    std::ranges::sort(people, {}, &Person::age);
    assert(people.front().name == "ann");
    std::unordered_set<int> u{1, 2, 2};
    assert(u.size() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/custom_comparator_hash_projection", run>;

}  // namespace
