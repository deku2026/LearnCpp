// Topic    : C++23 异质 erase/extract（P2077）
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 2.4（🆕）
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : heterogeneous_erase_extract_cpp23
// Topic id : part2/stage10/section02/heterogeneous_erase_extract_cpp23
// Refs     : https://en.cppreference.com/w/cpp/container/map/erase
//            https://en.cppreference.com/w/cpp/container/map/extract
//            https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2077r3.html
//            feature-test: __cpp_lib_associative_heterogeneous_erasure

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [heterogeneous_erase_extract_cpp23] 入门：C++20 缺口 ===\n";
    {
        // C++14/20：透明 find/contains 已可异质；但 erase(key)/extract(key)
        // 在 C++23 前仍要 key_type，可能构造临时 string。
        std::map<std::string, int, std::less<>> m{{"apple", 1}, {"banana", 2}};
        assert(m.contains(std::string_view{"apple"}));
        std::cout << "before P2077: heterogeneous lookup OK, erase often not\n";
    }

#if defined(__cpp_lib_associative_heterogeneous_erasure) && __cpp_lib_associative_heterogeneous_erasure >= 202110L

    std::cout << "=== 进阶：异质 erase(K&&)（P2077 可用）===\n";
    {
        std::map<std::string, int, std::less<>> m{
            {"apple", 1},
            {"banana", 2},
            {"cherry", 3},
        };
        // 直接用 string_view / const char* 删除，不造临时 string key
        [[maybe_unused]] const auto n = m.erase(std::string_view{"banana"});
        assert(n == 1);
        assert(!m.contains("banana"));
        assert(m.erase("nope") == 0);
        std::cout << "erase(string_view) without temporary std::string\n";
    }

    std::cout << "=== 进阶：异质 extract(K&&) ===\n";
    {
        std::map<std::string, int, std::less<>> m{
            {"x", 10},
            {"y", 20},
        };
        auto nh = m.extract(std::string_view{"x"});
        assert(!nh.empty());
        assert(nh.key() == "x");
        assert(nh.mapped() == 10);
        assert(!m.contains("x"));
        // 改 key 再插回
        nh.key() = "z";
        m.insert(std::move(nh));
        assert(m.contains("z") && m.at("z") == 10);
        std::cout << "extract(sv) → node_handle → reinsert\n";
    }

    std::cout << "=== 专家：set 同样支持 ===\n";
    {
        std::set<std::string, std::less<>> s{"red", "green", "blue"};
        assert(s.erase(std::string_view{"green"}) == 1);
        auto nh = s.extract(std::string_view{"blue"});
        assert(nh.value() == "blue");
        assert(!s.contains("blue"));
        std::cout << "set heterogeneous erase/extract OK\n";
    }

#else
    std::cout << "=== 进阶：本工具链无 P2077，演示等价旧写法 ===\n";
    {
        std::map<std::string, int, std::less<>> m{
            {"apple", 1},
            {"banana", 2},
            {"cherry", 3},
        };
        // 旧：必须传 key_type，或先 find 再 erase(iterator)
        if (auto it = m.find(std::string_view{"banana"}); it != m.end()) {
            m.erase(it);  // iterator 重载，无临时 key 分配于 erase 本身
        }
        assert(!m.contains("banana"));

        // extract 同理：先异质 find，再 extract(iterator)
        if (auto it = m.find(std::string_view{"apple"}); it != m.end()) {
            auto nh = m.extract(it);
            assert(nh.key() == "apple");
            nh.key() = "apricot";
            m.insert(std::move(nh));
        }
        assert(m.contains("apricot"));
        std::cout << "fallback: find(sv) + erase/extract(iterator)\n";
        std::cout << "feature macro __cpp_lib_associative_heterogeneous_erasure "
                     "not set\n";
    }
#endif

    std::cout << "=== 专家：为何仍要透明比较器 ===\n";
    {
        // 异质 erase/extract 同样要求 Compare::is_transparent
        std::map<std::string, int, std::less<>> transparent{{"k", 1}};
        std::map<std::string, int> opaque{{"k", 1}};
        assert(transparent.contains(std::string_view{"k"}));
        // opaque.contains(string_view) 在某些实现也可能走构造；
        // 可移植写法：opaque.contains(std::string{"k"})
        assert(opaque.contains(std::string{"k"}));
        std::cout << "transparent comparator remains the opt-in switch\n";
    }

    std::cout << "[heterogeneous_erase_extract_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section02/heterogeneous_erase_extract_cpp23", run>;

}  // namespace
