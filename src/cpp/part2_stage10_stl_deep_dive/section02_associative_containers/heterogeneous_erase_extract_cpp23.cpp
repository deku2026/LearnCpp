// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : heterogeneous_erase_extract_cpp23
// Topic id : part2/stage10/section02_associative_containers/heterogeneous_erase_extract_cpp23
//
// Covers: C++23 heterogeneous erase/extract with transparent comparator (P2077)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <map>
#include <string>
#include <string_view>
#include <version>

namespace {

void demo_basics() {
    std::map<std::string, int, std::less<>> m{{"a", 1}, {"b", 2}};
#if defined(__cpp_lib_associative_heterogeneous_erasure)
    assert(m.erase(std::string_view{"a"}) == 1);
#else
    assert(m.erase(std::string{"a"}) == 1);
#endif
    assert(!m.contains("a"));
    assert(m.contains("b"));
}

void demo_intermediate() {
    std::map<std::string, int, std::less<>> m{{"x", 10}, {"y", 20}};
#if defined(__cpp_lib_associative_heterogeneous_erasure)
    auto nh = m.extract(std::string_view{"x"});
#else
    auto nh = m.extract(std::string{"x"});
#endif
    assert(nh);
    assert(nh.key() == "x");
    assert(nh.mapped() == 10);
    assert(m.size() == 1);
    nh.key() = "z";
    m.insert(std::move(nh));
    assert(m.contains("z"));
}

void demo_expert() {
    std::map<std::string, int, std::less<>> m{{"keep", 1}, {"drop", 2}};
#if defined(__cpp_lib_associative_heterogeneous_erasure)
    const auto n = m.erase(std::string_view{"missing"});
    assert(n == 0);
    assert(m.erase("drop") == 1);
#else
    assert(m.erase(std::string{"missing"}) == 0);
    assert(m.erase(std::string{"drop"}) == 1);
#endif
    assert(m.size() == 1);
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
    ::learn::topic<"part2/stage10/section02_associative_containers/heterogeneous_erase_extract_cpp23", run>;

}  // namespace
