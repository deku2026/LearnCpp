// Topic     : 函数重载与重载决议入门
// Doc       : 第2部分-阶段3 · 步骤 3.2–3.3
// cppreference: https://en.cppreference.com/cpp/language/overload_resolution
//
// 要点: 同名不同参数列表可重载；返回类型不参与重载；
//       决议三步：候选 → 可行 → 最佳匹配；转换序列：精确 > 提升 > 标准转换 > 用户定义；
//       默认实参 + 重载易歧义。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

enum class Which { Int, Double, CStr, Bool, LongAmbiguous };

[[maybe_unused]] Which print(int) {
    return Which::Int;
}
[[maybe_unused]] Which print(double) {
    return Which::Double;
}
[[maybe_unused]] Which print(const char*) {
    return Which::CStr;
}

// 转换序列演示：char 提升到 int 优于转到 double
[[maybe_unused]] Which take_num(int) {
    return Which::Int;
}
[[maybe_unused]] Which take_num(double) {
    return Which::Double;
}

// 用户定义转换（最差一档，仅当无更好匹配时）
struct Meter {
    int v = 0;
    operator int() const { return v; }
};

[[maybe_unused]] Which take_int(int) {
    return Which::Int;
}

// 返回类型不参与重载——下列不能同时存在：
// int only_ret();
// double only_ret();

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [overload_resolution_intro] ===\n";

    // -------------------------------------------------------------------------
    // §入门：按参数类型选择重载
    // -------------------------------------------------------------------------
    assert(print(42) == Which::Int);
    assert(print(3.14) == Which::Double);
    assert(print("hello") == Which::CStr);
    std::cout << "[intro] print(int/double/const char*) selected by argument type\n";

    // -------------------------------------------------------------------------
    // §进阶：转换序列排名
    // -------------------------------------------------------------------------
    // 精确匹配
    assert(take_num(42) == Which::Int);
    assert(take_num(42.0) == Which::Double);

    // char → int 是整型提升；char → double 是标准转换；提升更优
    assert(take_num('a') == Which::Int);

    // bool → int 也是提升路径上的常见选择（实现上 bool 提升到 int）
    assert(take_num(true) == Which::Int);

    // 用户定义转换：Meter → int
    [[maybe_unused]] Meter m{7};
    assert(take_int(m) == Which::Int);

    std::cout << "[advanced] exact > promotion > standard conversion > user-defined\n";

    // -------------------------------------------------------------------------
    // §专家：歧义、nullptr、读候选列表
    // -------------------------------------------------------------------------
    // long 到 int / double 都可能是转换 → 平台上常歧义，不要依赖。
    // 这里用可编译的安全例子：nullptr 匹配指针更佳
    assert(print(static_cast<const char*>(nullptr)) == Which::CStr);

    // 默认实参与重载：void f(int); void f(int,int=0); → f(1) 歧义
    // 修复：删掉其中一个，或去掉默认、让调用者显式传第二参数。

    // 决议失败时读编译器「候选列表」：标出每个为什么被排除/为何并列最佳。
    // 成员函数还有 this 的隐式对象参数；模板还有更具体优先——阶段 6 / 分支 D 深讲。

    static_assert(!std::is_same_v<Which, int>);
    assert(take_int(Meter{3}) == Which::Int);

    std::cout << "[expert] ambiguity traps; read compiler candidate lists when resolution fails\n";
    std::cout << "=== overload_resolution_intro: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/overload_resolution_intro", run>;

}  // namespace
