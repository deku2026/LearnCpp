// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_dangling_pitfall
// Topic id : part2/stage10/section06_modern_utility_types/string_view_dangling_pitfall
//
// Covers: string_view dangling when underlying storage dies

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace {

std::vector<std::string_view> split(std::string_view sv, char delim) {
    std::vector<std::string_view> tokens;
    std::size_t start = 0;
    while (start <= sv.size()) {
        const auto end = sv.find(delim, start);
        if (end == std::string_view::npos) {
            tokens.push_back(sv.substr(start));
            break;
        }
        tokens.push_back(sv.substr(start, end - start));
        start = end + 1;
    }
    return tokens;
}

void demo_basics() {
    const std::string s = "a,b,c";
    auto tokens = split(s, ',');
    LEARN_CHECK(tokens.size() == 3);
    LEARN_CHECK(tokens[0] == "a");
    LEARN_CHECK(tokens[2] == "c");
    // tokens remain valid only while s lives
}

void demo_intermediate() {
    std::string owner = "stable";
    std::string_view sv = owner;
    LEARN_CHECK(sv == "stable");
    owner = "changed-longer-string";  // may reallocate
    // sv may dangle after reallocation — do not use sv; rebind instead
    sv = owner;
    LEARN_CHECK(sv == "changed-longer-string");
}

void demo_expert() {
    // Safe: view into static storage
    static const std::string persistent = "ok";
    std::string_view sv = persistent;
    LEARN_CHECK(sv == "ok");
    // Unsafe patterns (not executed): return string_view to local/temporary
    // std::string_view bad = std::string("temp"); // temporary dies
    LEARN_CHECK(!sv.empty());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06_modern_utility_types/string_view_dangling_pitfall", run>;

}  // namespace
