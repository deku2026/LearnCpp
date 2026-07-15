// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : transparent_comparator_heterogeneous_lookup
// Topic id : part2/stage10/section02_associative_containers/transparent_comparator_heterogeneous_lookup
//
// Covers: transparent less<> / hash for heterogeneous lookup without temporary key

#include "learn/topic_registry.hpp"

#include <map>
#include <string>
#include <string_view>
#include <unordered_map>

namespace {

struct StringHash {
    using is_transparent = void;
    std::size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }
};

void demo_basics() {
    std::map<std::string, int, std::less<>> m{{"apple", 1}, {"banana", 2}};
    auto it = m.find("apple");  // const char*, no temporary string required
    LEARN_CHECK(it != m.end());
    LEARN_CHECK(it->second == 1);
    LEARN_CHECK(m.find(std::string_view{"banana"})->second == 2);
}

void demo_intermediate() {
    std::map<std::string, int, std::less<>> m{{"x", 10}};
    LEARN_CHECK(m.contains(std::string_view{"x"}));
    LEARN_CHECK(m.count("x") == 1);
    LEARN_CHECK(m.lower_bound("x")->second == 10);
}

void demo_expert() {
    std::unordered_map<std::string, int, StringHash, std::equal_to<>> um;
    um.emplace("key", 7);
    auto it = um.find(std::string_view{"key"});
    LEARN_CHECK(it != um.end());
    LEARN_CHECK(it->second == 7);
    LEARN_CHECK(um.contains("key"));
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section02_associative_containers/transparent_comparator_heterogeneous_lookup", run>;

}  // namespace
