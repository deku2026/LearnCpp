// Topic    : 透明比较器 / 异质查找（避免临时 string）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 2.3（验收点）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : transparent_comparator_heterogeneous_lookup
// Topic id : part2/stage10/section02/transparent_comparator_heterogeneous_lookup
// Refs     : https://en.cppreference.com/w/cpp/utility/functional/less_void
//            https://en.cppreference.com/w/cpp/container/map/find
//            https://en.cppreference.com/w/cpp/container/unordered_map/find
//            P0919R3 heterogeneous lookup for unordered containers

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>

namespace {

// C++20 unordered 异质查找：透明 Hash + 透明 KeyEqual
struct StringHash {
    using is_transparent = void;

    std::size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }
    std::size_t operator()(const std::string& s) const noexcept { return std::hash<std::string_view>{}(s); }
    std::size_t operator()(const char* s) const noexcept { return std::hash<std::string_view>{}(s); }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [transparent_comparator_heterogeneous_lookup] 入门：问题 ===\n";
    {
        // 默认 map::find 参数是 key_type → 从字面量会构造临时 string
        std::map<std::string, int> m{{"apple", 1}, {"banana", 2}};
        auto it = m.find(std::string("apple"));  // 显式临时
        assert(it != m.end() && it->second == 1);
        // m.find("apple") 在非透明比较器下同样会构造临时 string（key_type）
        std::cout << "default find(key_type) may construct temporary key\n";
    }

    std::cout << "=== 入门：std::less<> 启用有序容器异质查找 ===\n";
    {
        std::map<std::string, int, std::less<>> m{
            {"apple", 1},
            {"banana", 2},
        };
        // 直接用 const char* / string_view 比较，不造临时 string
        auto it1 = m.find("apple");
        auto it2 = m.find(std::string_view{"banana"});
        assert(it1 != m.end() && it1->second == 1);
        assert(it2 != m.end() && it2->second == 2);
        assert(m.contains(std::string_view{"apple"}));
        assert(m.count("nope") == 0);
        std::cout << "map<string,T,less<>>: find/count/contains heterogeneous\n";
    }

    std::cout << "=== 进阶：set 同样适用 ===\n";
    {
        std::set<std::string, std::less<>> s{"red", "green", "blue"};
        assert(s.contains(std::string_view{"green"}));
        auto it = s.lower_bound(std::string_view{"g"});
        assert(it != s.end());
        // lower_bound("g") 异质：不构造 string("g") 作为 key_type 临时
        std::cout << "set + less<>: lower_bound with string_view\n";
    }

    std::cout << "=== 进阶：unordered_map 透明 hash + equal_to<>（C++20）===\n";
    {
        std::unordered_map<std::string, int, StringHash, std::equal_to<>> um;
        um.emplace("alpha", 10);
        um.emplace("beta", 20);

        auto it = um.find(std::string_view{"alpha"});
        assert(it != um.end() && it->second == 10);
        assert(um.contains("beta"));
        assert(!um.contains(std::string_view{"gamma"}));
        std::cout << "unordered heterogeneous lookup needs transparent hash\n";
    }

    std::cout << "=== 专家：is_transparent 标记如何工作 ===\n";
    {
        // 比较器若嵌套 is_transparent，则 map 启用 templated find 重载
        static_assert(requires { typename std::less<>::is_transparent; });
        static_assert(requires { typename StringHash::is_transparent; });
        static_assert(requires { typename std::equal_to<>::is_transparent; });
        // 非透明：std::less<std::string> 没有 is_transparent
        std::cout << "is_transparent is the opt-in tag for heterogeneous APIs\n";
    }

    std::cout << "=== 专家：性能动机 — 热路径点查别付分配税 ===\n";
    {
        std::map<std::string, int, std::less<>> scores;
        for (int i = 0; i < 100; ++i) {
            scores.emplace("user_" + std::to_string(i), i);
        }
        // 热路径：从缓冲/网络拿到 string_view 直接查
        std::string_view key = "user_42";
        auto it = scores.find(key);
        assert(it != scores.end() && it->second == 42);
        std::cout << "hot path: lookup from string_view without alloc\n";
    }

    std::cout << "[transparent_comparator_heterogeneous_lookup] all checks "
                 "passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section02/transparent_comparator_heterogeneous_lookup", run>;

}  // namespace
