// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : std_hash_load_factor
// Topic id : part2/stage10/section02_associative_containers/std_hash_load_factor
//
// Covers: std::hash, buckets, load factor, rehash, custom hash

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>

namespace {

struct Point {
    int x;
    int y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};

struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
    }
};

void demo_basics() {
    LEARN_CHECK(std::hash<int>{}(42) == std::hash<int>{}(42));
    std::unordered_map<int, int> um;
    um[1] = 1;
    LEARN_CHECK(um.bucket_count() >= 1);
    LEARN_CHECK(um.load_factor() > 0.0f);
}

void demo_intermediate() {
    std::unordered_map<int, int> um;
    um.max_load_factor(1.0f);
    um.reserve(100);
    LEARN_CHECK(um.bucket_count() >= 100);
    for (int i = 0; i < 50; ++i) {
        um[i] = i;
    }
    LEARN_CHECK(um.load_factor() <= um.max_load_factor());
}

void demo_expert() {
    std::unordered_map<Point, int, PointHash> um;
    const Point key{1, 2};
    um[key] = 3;
    LEARN_CHECK(um[key] == 3);
    um.rehash(64);
    LEARN_CHECK(um.bucket_count() >= 64);
    LEARN_CHECK(um.at(key) == 3);
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
    ::learn::topic<"part2/stage10/section02_associative_containers/std_hash_load_factor", run>;

}  // namespace
