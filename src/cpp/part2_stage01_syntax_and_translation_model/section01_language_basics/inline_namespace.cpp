// Topic     : inline namespace（内联命名空间）
// Doc       : 阶段 1 命名空间延伸 / 库版本化
// cppreference: https://en.cppreference.com/cpp/language/namespace  (Inline namespaces)
//
// 要点: inline namespace 成员同时出现在内层与外层命名空间；用于 ABI/API 版本选择；
//       using 指令穿透；显式特化可写在父命名空间；与普通嵌套命名空间对比。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

// 模拟第三方库的"当前默认版本 + 旧版本并存"
namespace paintlib {
// v2 为当前默认：inline → paintlib::blend 即 paintlib::v2::blend
inline namespace v2 {
inline int api_level() {
    return 2;
}
inline std::string blend(std::string a, std::string b) {
    return a + "+" + b + "@v2";
}
template <class T>
struct Brush {
    static constexpr int version = 2;
    T tip{};
};
}  // namespace v2

// 旧版本仍可显式限定访问，但不 inline
namespace v1 {
inline int api_level() {
    return 1;
}
inline std::string blend(std::string a, std::string b) {
    return a + "|" + b + "@v1";
}
template <class T>
struct Brush {
    static constexpr int version = 1;
    T tip{};
};
}  // namespace v1
}  // namespace paintlib

// 用户在父命名空间为模板写显式特化（inline namespace 版本化场景的标准允许写法）
namespace paintlib {
template <>
struct Brush<int> {
    static constexpr int version = 2;
    int tip = 7;
};
}  // namespace paintlib

// 对比：非 inline 嵌套不会把名字提升到外层
namespace toolbox {
namespace nested {
constexpr int k = 3;
}
// 若写成 inline namespace nested，则 toolbox::k 可用
}  // namespace toolbox

// 标准库熟悉场景：std::literals 下的 inline 成员命名空间（string_literals 等）
// using namespace std::string_literals; 能看见 operator""s，正因为 inline 提升。

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [inline_namespace] ===\n";

    // -------------------------------------------------------------------------
    // §入门：inline 使内层名字成为外层的成员
    // -------------------------------------------------------------------------
    assert(paintlib::api_level() == 2);      // 来自 inline v2
    assert(paintlib::v2::api_level() == 2);  // 也可显式限定
    assert(paintlib::v1::api_level() == 1);  // 旧版必须带 v1::

    const auto mixed = paintlib::blend("red", "blue");
    assert(mixed == "red+blue@v2");
    const auto legacy = paintlib::v1::blend("red", "blue");
    assert(legacy == "red|blue@v1");
    std::cout << "[intro] default blend=" << mixed << " legacy=" << legacy << '\n';

    // -------------------------------------------------------------------------
    // §进阶：using 与类型别名路径
    // -------------------------------------------------------------------------
    {
        using namespace paintlib;  // 打开外层，一并看见 inline 成员
        assert(api_level() == 2);
        Brush<char> br{};
        br.tip = 'X';
        assert(Brush<char>::version == 2);
        assert(br.tip == 'X');
    }

    // 非 inline 嵌套：外层没有 k
    assert(toolbox::nested::k == 3);
    // toolbox::k 不存在——对比 paintlib::api_level
    std::cout << "[advanced] non-inline nested names stay qualified only\n";

    // 显式特化落在父命名空间
    paintlib::Brush<int> bi{};
    assert(bi.tip == 7);
    assert(paintlib::Brush<int>::version == 2);
    std::cout << "[advanced] explicit specialization in parent ns works with inline versioning\n";

    // -------------------------------------------------------------------------
    // §专家：版本化意图与边界
    // -------------------------------------------------------------------------
    // 1) 链接符号带内层命名空间修饰：v1::blend 与 v2::blend mangling 不同，
    //    可降低"用旧头 + 新库"时静默 ODR/ABI 错配的概率（非万能）。
    // 2) 同一时刻通常只 inline 一个"当前"版本；切换默认版本 = 改谁 inline。
    // 3) inline 关键字在此与函数 inline（放宽 ODR）是不同机制，勿混淆。
    // 4) 标准库例: std::chrono_literals / string_literals 位于 inline 嵌套中，
    //    故 using namespace std::literals; 能一次引入多组运算符。
    // 5) 坑: 过度依赖 inline 版本化无法单独解决全部 ABI 问题（虚表布局、
    //    内联函数体变化等仍可运行期炸）；库演进还需稳定 ABI 纪律 / SONAME 等。
    //
    // 类型关系：paintlib::Brush<char> 与 paintlib::v2::Brush<char> 是同一类型
    static_assert(std::is_same_v<paintlib::Brush<char>, paintlib::v2::Brush<char>>);
    static_assert(!std::is_same_v<paintlib::v1::Brush<char>, paintlib::v2::Brush<char>>);

    std::cout << "[expert] inline ns = transparent nesting for lookup + distinct mangling path\n";
    std::cout << "=== inline_namespace: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/inline_namespace", run>;

}  // namespace
