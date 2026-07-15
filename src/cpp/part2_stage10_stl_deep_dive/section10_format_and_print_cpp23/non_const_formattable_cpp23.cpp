// Topic    : C++23 非 const 可格式化类型(P2418) — generator-like 友好
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 15.2③
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : non_const_formattable_cpp23
// Topic id : part2/stage10/section10/non_const_formattable_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/format/formatter
//            P2418R2 — formatting of non-const-iterable types

#include "learn/topic_registry.hpp"

#include <cassert>
#include <format>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

// 模拟「只能非 const 迭代」的惰性序列(类似 generator 的 input_range)
// begin() 仅非 const 可用 → C++20 format 若要求 const 范围会失败;
// C++23 P2418 允许对非 const lvalue/rvalue 做格式化。
class OnceSeq {
public:
    explicit OnceSeq(std::vector<int> data) : data_(std::move(data)) {}

    // 故意不提供 const begin/end
    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }

private:
    std::vector<int> data_;
};

// 自定义类型: formatter 的 format 接受非 const T&(演示 API 形状)
struct MutableLabel {
    std::string text;
    int touch_count = 0;

    // 非 const 访问会修改缓存计数 —— 格式化时允许非 const
    std::string_view view() {
        ++touch_count;
        return text;
    }
};

}  // namespace

// formatter 必须在 std 命名空间特化
template <>
struct std::formatter<MutableLabel> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    // format 形参是 const MutableLabel& 在 C++20 常见;
    // 对「必须非 const」的成员, 用 const_cast 不优雅 ——
    // 这里用可 const 的 text 字段输出, 另演示 formattable 对临时/非 const 的放宽在 ranges 侧。
    auto format(const MutableLabel& v, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "Label({})", v.text);
    }
};

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [non_const_formattable_cpp23] P2418 spirit ===\n";

    // ① 自定义 formatter: 用户类型直接 format
    {
        MutableLabel lab{"alpha"};
        const std::string s = std::format("{}", lab);
        assert(s == "Label(alpha)");
        std::cout << "  custom formatter: " << s << '\n';
    }

    // ② 临时/非 const 对象可格式化
    {
        const std::string s = std::format("{}", MutableLabel{"beta"});
        assert(s == "Label(beta)");
        std::cout << "  prvalue formattable: " << s << '\n';
    }

#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L
    // ③ 非 const 可迭代范围: format 接受非 const lvalue(P2418 动机)
    {
        OnceSeq seq(std::vector<int>{1, 2, 3});
        // 非 const lvalue 范围 — C++23 允许
        const std::string s = std::format("{}", seq);
        assert(s == "[1, 2, 3]");
        std::cout << "  non-const-iterable range: " << s << '\n';
    }
    // 临时容器(拥有元素)也可
    {
        const std::string s = std::format("{}", std::vector<int>{4, 5});
        assert(s == "[4, 5]");
        std::cout << "  temporary vector: " << s << '\n';
    }
#else
    {
        OnceSeq seq(std::vector<int>{1, 2, 3});
        std::string s = "[";
        bool first = true;
        for (int x : seq) {
            if (!first) {
                s += ", ";
            }
            first = false;
            s += std::format("{}", x);
        }
        s += "]";
        assert(s == "[1, 2, 3]");
        std::cout << "  (no format_ranges) manual non-const iter: " << s << '\n';
    }
#endif

    std::cout << "[non_const_formattable_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section10/non_const_formattable_cpp23", run>;

}  // namespace
