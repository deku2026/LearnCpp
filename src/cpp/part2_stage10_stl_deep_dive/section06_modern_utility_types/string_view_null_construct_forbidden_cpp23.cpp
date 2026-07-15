// Topic     : string_view 禁止 nullptr 构造 (C++23, P2166)
// Doc       : 第2部分-阶段10 · 步骤 6.3
// cppreference: https://en.cppreference.com/cpp/string/basic_string_view/basic_string_view
//
// 要点: nullptr_t 构造 = delete; 编译期拦截, 避免运行期 UB。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_view_null_construct_forbidden_cpp23] ===\n";

    // ✅ 合法构造
    std::string_view a{"hello"};
    std::string s = "owned";
    std::string_view b{s};
    std::string_view c{s.data(), s.size()};
    assert(a == "hello" && b == "owned" && c == "owned");

    // ❌ C++23: 下列代码应无法编译(不要取消注释)
    // std::string_view bad = nullptr;
    // std::string_view bad2{nullptr};

    // 用类型特征说明 nullptr_t 构造不可用
    static_assert(!std::is_constructible_v<std::string_view, std::nullptr_t>);
    static_assert(!std::is_convertible_v<std::nullptr_t, std::string_view>);

    // 空 view 用默认构造或 "" —— 不是 nullptr
    std::string_view empty{};
    assert(empty.empty());
    assert(empty.data() == nullptr || empty.size() == 0);
    std::string_view empty2{""};
    assert(empty2.empty());

    // 从可能为空的 C 字符串: 必须先判空再构造
    const char* p = "ok";
    const char* q = nullptr;
    auto make_sv = [](const char* ptr) -> std::string_view { return ptr ? std::string_view{ptr} : std::string_view{}; };
    assert(make_sv(p) == "ok");
    assert(make_sv(q).empty());

    std::cout << "[P2166] nullptr construct deleted; empty via default/\"\"\n";
    std::cout << "string_view_null_construct_forbidden_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06/string_view_null_construct_forbidden_cpp23", run>;

}  // namespace
