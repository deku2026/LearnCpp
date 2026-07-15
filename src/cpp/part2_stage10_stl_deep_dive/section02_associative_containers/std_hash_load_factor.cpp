// Topic    : std::hash / 桶 / load_factor / rehash 机制
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 2.2
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : std_hash_load_factor
// Topic id : part2/stage10/section02/std_hash_load_factor
// Refs     : https://en.cppreference.com/w/cpp/utility/hash
//            https://en.cppreference.com/w/cpp/container/unordered_map/load_factor
//            https://en.cppreference.com/w/cpp/container/unordered_map/rehash
//            ISO [unord.hash] [unord.req]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace {

struct Person {
    std::string name;
    int id = 0;
    friend bool operator==(const Person& a, const Person& b) { return a.id == b.id && a.name == b.name; }
};

}  // namespace

// 自定义类型哈希：必须放在 std 特化或作为模板参数传入
template <>
struct std::hash<Person> {
    std::size_t operator()(const Person& p) const noexcept {
        const std::size_t h1 = std::hash<std::string>{}(p.name);
        const std::size_t h2 = std::hash<int>{}(p.id);
        // 常见 mix：避免 h1^h2 在对称输入时撞车过多
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_hash_load_factor] 入门：std::hash 把 key → size_t ===\n";
    {
        const std::size_t h1 = std::hash<int>{}(42);
        const std::size_t h2 = std::hash<std::string>{}("hello");
        [[maybe_unused]] const std::size_t h3 = std::hash<std::string_view>{}("hello");
        (void)h1;
        // 同值可哈希多次：相等对象哈希必须相等
        assert(std::hash<int>{}(42) == std::hash<int>{}(42));
        assert(h2 == h3);
        std::cout << "hash(42)=" << h1 << " hash(\"hello\")=" << h2 << '\n';
    }

    std::cout << "=== 入门：load_factor = size / bucket_count ===\n";
    {
        std::unordered_map<int, int> um;
        um.max_load_factor(0.5f);  // 更早 rehash，桶更多、冲突更少（空间换时间）
        for (int i = 0; i < 20; ++i) {
            um.emplace(i, i);
        }
        const float lf = um.load_factor();
        assert(lf <= um.max_load_factor() + 1e-5f);
        std::cout << "size=" << um.size() << " buckets=" << um.bucket_count() << " load=" << lf
                  << " max=" << um.max_load_factor() << '\n';
    }

    std::cout << "=== 进阶：观察 bucket 分布（冲突直观）===\n";
    {
        std::unordered_map<int, int> um;
        um.rehash(8);  // 固定较小桶数便于观察
        for (int i = 0; i < 16; ++i) {
            um.emplace(i, i);
        }
        std::size_t max_bucket = 0;
        for (std::size_t b = 0; b < um.bucket_count(); ++b) {
            max_bucket = std::max(max_bucket, um.bucket_size(b));
            if (um.bucket_size(b) > 0) {
                std::cout << "  bucket[" << b << "] size=" << um.bucket_size(b) << '\n';
            }
        }
        assert(max_bucket >= 1);
        std::cout << "max chain length=" << max_bucket << '\n';
    }

    std::cout << "=== 进阶：reserve 避免插入中途多次 rehash ===\n";
    {
        std::unordered_map<int, int> a;
        std::unordered_map<int, int> b;
        b.reserve(1000);

        std::size_t rehash_like_a = 0;
        std::size_t last_a = a.bucket_count();
        for (int i = 0; i < 1000; ++i) {
            a.emplace(i, i);
            if (a.bucket_count() != last_a) {
                ++rehash_like_a;
                last_a = a.bucket_count();
            }
        }

        std::size_t rehash_like_b = 0;
        std::size_t last_b = b.bucket_count();
        for (int i = 0; i < 1000; ++i) {
            b.emplace(i, i);
            if (b.bucket_count() != last_b) {
                ++rehash_like_b;
                last_b = b.bucket_count();
            }
        }
        std::cout << "bucket growth events: no-reserve=" << rehash_like_a << " reserved=" << rehash_like_b << '\n';
        assert(rehash_like_b <= rehash_like_a);
    }

    std::cout << "=== 专家：特化 std::hash<Person> ===\n";
    {
        std::unordered_map<Person, std::string> db;
        db.emplace(Person{"Ada", 1}, "lovelace");
        db.emplace(Person{"Grace", 2}, "hopper");
        assert(db.at(Person{"Ada", 1}) == "lovelace");
        // 相等 ⇒ 哈希相等（质量要求）；反过来不必
        assert(std::hash<Person>{}(Person{"Ada", 1}) == std::hash<Person>{}(Person{"Ada", 1}));
        std::cout << "custom hash specialization enables unordered key\n";
    }

    std::cout << "=== 专家：坏哈希的灾难（全进同一桶）===\n";
    {
        struct BadHash {
            std::size_t operator()(int) const noexcept { return 0; }
        };
        std::unordered_map<int, int, BadHash> poisoned;
        for (int i = 0; i < 100; ++i) {
            poisoned.emplace(i, i);
        }
        // 所有元素落到同一桶 → 查找退化链表 O(n)
        assert(poisoned.bucket_count() >= 1);
        std::size_t max_sz = 0;
        for (std::size_t b = 0; b < poisoned.bucket_count(); ++b) {
            max_sz = std::max(max_sz, poisoned.bucket_size(b));
        }
        assert(max_sz == 100);
        std::cout << "constant hash → one bucket holds all (O(n) lookups)\n";
    }

    std::cout << "[std_hash_load_factor] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section02/std_hash_load_factor", run>;

}  // namespace
