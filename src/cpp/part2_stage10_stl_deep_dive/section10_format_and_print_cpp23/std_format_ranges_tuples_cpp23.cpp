// Topic    : C++23 format ranges / tuples / 容器(P2286)
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 15.2①
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_ranges_tuples_cpp23
// Topic id : part2/stage10/section10/std_format_ranges_tuples_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/format/range_formatter
//            https://en.cppreference.com/w/cpp/utility/format/tuple_formatter
//            P2286R8  Feature-test: __cpp_lib_format_ranges

#include "learn/topic_registry.hpp"

#include <format>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_format_ranges_tuples_cpp23] format containers ===\n";

#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L
    std::cout << "  __cpp_lib_format_ranges=" << __cpp_lib_format_ranges << '\n';

    // ① vector → [1, 2, 3]
    {
        std::vector<int> v{1, 2, 3};
        const std::string s = std::format("{}", v);
        assert(s == "[1, 2, 3]");
        std::cout << "  vector: " << s << '\n';
    }

    // ② 每元素说明符: {::02d} → [01, 02, 03]
    {
        std::vector<int> v{1, 2, 3};
        const std::string s = std::format("{::02d}", v);
        assert(s == "[01, 02, 03]");
        std::cout << "  per-element: " << s << '\n';
    }

    // ③ :n 去掉方括号
    {
        std::vector<int> v{1, 2, 3};
        const std::string s = std::format("{:n}", v);
        assert(s == "1, 2, 3");
        std::cout << "  :n bare: " << s << '\n';
    }

    // ④ map / pair-like
    {
        std::map<std::string, int> m{{"a", 1}, {"b", 2}};
        const std::string s = std::format("{}", m);
        // 典型: {"a": 1, "b": 2}  (有序 map)
        assert(s.find("a") != std::string::npos);
        assert(s.find("1") != std::string::npos);
        std::cout << "  map: " << s << '\n';
    }

    // ⑤ tuple
    {
        std::tuple t{std::string{"x"}, 42, true};
        const std::string s = std::format("{}", t);
        assert(s.find("x") != std::string::npos);
        assert(s.find("42") != std::string::npos);
        std::cout << "  tuple: " << s << '\n';
    }

    // ⑥ pair
    {
        std::pair p{std::string{"k"}, 9};
        const std::string s = std::format("{}", p);
        assert(s.find("k") != std::string::npos);
        assert(s.find("9") != std::string::npos);
        std::cout << "  pair: " << s << '\n';
    }

#else
    // 回退: 手动拼容器字符串, 语义对齐教学
    {
        std::vector<int> v{1, 2, 3};
        std::string s = "[";
        for (std::size_t i = 0; i < v.size(); ++i) {
            if (i) {
                s += ", ";
            }
            s += std::format("{}", v[i]);
        }
        s += "]";
        assert(s == "[1, 2, 3]");
        std::cout << "  (no format_ranges) manual: " << s << '\n';
    }
    {
        std::tuple t{std::string{"x"}, 42};
        const std::string s = std::format("({}, {})", std::get<0>(t), std::get<1>(t));
        assert(s == "(x, 42)");
        std::cout << "  manual tuple: " << s << '\n';
    }
#endif

    std::cout << "[std_format_ranges_tuples_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section10/std_format_ranges_tuples_cpp23", run>;

}  // namespace
