// Topic     : lvalue / xvalue / prvalue 三分
// Doc       : 第2部分-阶段2 · 步骤 5.2–5.3
// cppreference: https://en.cppreference.com/cpp/language/value_category
//
// 要点: 每个表达式恰属 lvalue、xvalue、prvalue 之一；
//       有身份? + 可移动? 两问定位；字符串字面值是 lvalue。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 用重载粗测“绑定到哪一类引用”（教学用近似：仅 int）
int classify(const int&) {
    return 1;
}  // lvalue → const&
int classify(int&&) {
    return 2;
}  // xvalue/prvalue → &&

// 字符串版：同样展示“具名 && 仍是 lvalue”
int classify_str(const std::string&) {
    return 1;
}
int classify_str(std::string&&) {
    return 2;
}

std::string get_string() {
    return "prvalue";
}
std::string& get_lref(std::string& s) {
    return s;
}
std::string&& get_rref(std::string&& s) {
    return std::move(s);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [lvalue_xvalue_prvalue] ===\n";

    // -------------------------------------------------------------------------
    // 入门：两问模型
    // -------------------------------------------------------------------------
    //              有身份(可取址)   可移动
    // lvalue         是              否
    // xvalue         是              是
    // prvalue        否              是

    std::string s = "hi";
    int a = 0;
    int arr[3]{};

    // lvalue：变量、下标、前置 ++、返回左值引用、字符串字面值
    (void)s;
    (void)arr[0];
    (void)++a;
    assert(a == 1);
    std::string& lr = get_lref(s);
    assert(&lr == &s);
    const char (&lit)[3] = "hi";  // 字符串字面值是 lvalue，有静态地址
    assert(lit[0] == 'h');
    std::cout << "[intro] lvalues have identity and are not movable as resources\n";

    // -------------------------------------------------------------------------
    // 进阶：prvalue 与 xvalue 例子
    // -------------------------------------------------------------------------
    // prvalue：除字符串外的字面值、内置算术结果、按值返回、临时量构造
    assert(classify(a) == 1);      // lvalue → const&
    assert(classify(42) == 2);     // prvalue → &&
    assert(classify(a + 1) == 2);  // prvalue
    auto tmp = get_string();
    assert(tmp == "prvalue");

    // xvalue：std::move 结果、返回右值引用的调用、转成 T&& 的 cast
    assert(classify(std::move(a)) == 2);
    std::string material = "x";
    std::string&& xr = get_rref(std::move(material));
    assert(xr == "x");

    // 具名右值引用变量是 lvalue！
    std::string&& named = std::string{"n"};
    assert(classify_str(named) == 1);             // 具名 && 是 lvalue → const&
    assert(classify_str(std::move(named)) == 2);  // 再变成 xvalue
    std::cout << "[advanced] move/cast → xvalue; named && variable is still lvalue\n";

    // -------------------------------------------------------------------------
    // 专家：反直觉清单
    // -------------------------------------------------------------------------
    // 1) "abc" 是 lvalue；42 是 prvalue
    // 2) std::move 不移动，只改值类别
    // 3) 返回值优化 / 物化见 prvalue_materialization_cpp17
    // 4) decltype((x)) 对 lvalue 得 T& —— 与值类别挂钩

    int n = 0;
    static_assert(std::is_same_v<decltype((n)), int&>);  // (n) 是 lvalue 表达式
    static_assert(std::is_same_v<decltype(std::move(n)), int&&>);
    static_assert(std::is_same_v<decltype(1 + 2), int>);  // prvalue → T

    std::cout << "[expert] value category is independent of the type of the expression\n";
    std::cout << "=== lvalue_xvalue_prvalue: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/lvalue_xvalue_prvalue", run>;

}  // namespace
