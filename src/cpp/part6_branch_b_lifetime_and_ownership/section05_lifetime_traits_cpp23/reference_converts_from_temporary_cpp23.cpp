// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B8 converts_from_temporary)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : reference_converts_from_temporary_cpp23
// Topic id : part6/b/section05/reference_converts_from_temporary_cpp23
//
// 要点: converts 覆盖更宽——含隐式转换路径产生临时（如 const string& ← const char*）。
// 参考: https://en.cppreference.com/w/cpp/types/reference_converts_from_temporary  P2255

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

namespace {

// 进阶: 接口用 string_view / 按值，避免 const string& 吃 C 串时造临时
[[maybe_unused]] std::size_t use_view(std::string_view sv) {
    return sv.size();
}
[[maybe_unused]] std::size_t use_owned(std::string s) {
    return s.size();
}

// 危险 API 形态（教学）: 存 const string& 成员却接受可转换类型
// struct Bad { const std::string& s; }; // Bad b{"hi"}; 临时 string 悬垂

template <class Ref, class From>
void explain() {
#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    constexpr bool c = std::reference_constructs_from_temporary_v<Ref, From>;
    constexpr bool v = std::reference_converts_from_temporary_v<Ref, From>;
    std::cout << "  constructs=" << c << " converts=" << v << '\n';
    (void)c;
    (void)v;
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B8 reference_converts_from_temporary (C++23) ===\n";

#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    // --- 入门: char* → 临时 string → 绑 const string& ---
    static_assert(std::reference_converts_from_temporary_v<const std::string&, const char*>);
    static_assert(std::reference_converts_from_temporary_v<const std::string&, std::string>);
    static_assert(!std::reference_converts_from_temporary_v<const std::string&, const std::string&>);
    static_assert(!std::reference_converts_from_temporary_v<const std::string&, std::string&>);

    assert((std::reference_converts_from_temporary_v<const std::string&, const char*>));
    assert((!std::reference_converts_from_temporary_v<const std::string&, const std::string&>));

    // --- 进阶: constructs vs converts 对照 ---
    // const char* → string 需要转换，converts 捕获；constructs 语义略窄（实现以标准为准）
    std::cout << "  const string& <- const char*:\n";
    explain<const std::string&, const char*>();
    std::cout << "  const string& <- string:\n";
    explain<const std::string&, std::string>();
    std::cout << "  const string& <- const string&:\n";
    explain<const std::string&, const std::string&>();

    // --- 安全 API ---
    assert(use_view("hello") == 5);  // 字面量 → string_view，无堆临时 string 悬挂问题
    std::string s = "world";
    assert(use_view(s) == 5);
    assert(use_owned("x") == 1);  // 按值：拥有副本

    // 专家: 库接口优先 string_view / span，少用 const string& 作「观察」参数
    // （const string& 会从 const char* 静默造临时，寿命仅 full-expression）
    std::cout << "  converts catches implicit temporary (e.g. char*→string)\n";
    std::cout << "  prefer string_view for non-owning string params\n";
#else
    std::cout << "  trait not available on this library; semantic fallback\n";
    assert(use_view("hello") == 5);
#endif

    std::cout << "reference_converts_from_temporary_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section05/reference_converts_from_temporary_cpp23", run>;

}  // namespace
