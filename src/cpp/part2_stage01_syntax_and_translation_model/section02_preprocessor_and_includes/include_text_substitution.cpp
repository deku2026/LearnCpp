// Topic     : #include 的文本替换本质
// Doc       : 第2部分-阶段1 · 步骤 3
// cppreference: https://en.cppreference.com/cpp/preprocessor/include
//               https://en.cppreference.com/cpp/language/translation_phases
//
// 要点: #include 在阶段 4 做整段文本粘贴；<> vs "" 搜索路径；递归展开；
//       预处理器不懂 C++ 语法；__has_include；-E / /P 实证命令。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

// 模拟"头文件内容"：在真实工程中这些会在 .hpp 里，被 #include 粘贴进来。
// 这里用宏 + 立即文本展示"粘贴后如同写在此处"。
#define LEARN_STAGE01_FAKE_HEADER_BODY    \
    namespace fake_hdr {                  \
    inline constexpr int kMagic = 0xC0DE; \
    inline int twice(int x) {             \
        return x * 2;                     \
    }                                     \
    }

LEARN_STAGE01_FAKE_HEADER_BODY

// 演示宏是纯文本替换（预处理器"笨"）——文档 §3.6 三类陷阱
#define NAIVE_SQUARE(x) x* x
#define SAFE_SQUARE(x) ((x) * (x))
// 陷阱②：参数在展开中出现多次 → 副作用被求值多次（逻辑错 / 曾接近 UB）
#define NAIVE_MAX(a, b) ((a) > (b) ? (a) : (b))
// 陷阱③：对象式宏无作用域，会污染后续一切同名记号（Windows min/max 经典坑）
// 绝不在头文件里 #define max/min；需要时用 std::max 或局部函数。

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#define LEARN_HAS_VERSION_HEADER 1
#else
#define LEARN_HAS_VERSION_HEADER 0
#endif
#else
#define LEARN_HAS_VERSION_HEADER 0
#endif

namespace {

// 函数式替代：参数只求值一次，有类型、有作用域
constexpr int max_i(int a, int b) {
    return a > b ? a : b;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [include_text_substitution] ===\n";

    // -------------------------------------------------------------------------
    // §入门：#include 做了什么
    // -------------------------------------------------------------------------
    // #include <header>  /  #include "header"
    // = 把目标文件全部内容复制到该指令所在位置，然后对粘进来的内容递归走阶段 1–4。
    // 尖括号：系统/标准库搜索路径；引号：通常先当前文件目录，再退回系统路径。
    // 本 TU 已 #include "learn/topic_registry.hpp" 与 <cassert> 等——它们的声明
    // 之所以可见，全因预处理文本粘贴，而非"链接时自动找到头"。

    assert(fake_hdr::kMagic == 0xC0DE);
    assert(fake_hdr::twice(21) == 42);
    std::cout << "[intro] after textual inclusion, fake_hdr entities are ordinary names\n";

    // -------------------------------------------------------------------------
    // §进阶：预处理器不懂语法 → 宏陷阱（include 同属阶段 4）
    // -------------------------------------------------------------------------
    // 陷阱①：不加括号 → 运算符优先级
    const int a = NAIVE_SQUARE(1 + 2);  // 1 + 2 * 1 + 2 = 5
    const int b = SAFE_SQUARE(1 + 2);   // ((1+2)*(1+2)) = 9
    assert(a == 5);
    assert(b == 9);
    std::cout << "[pitfall1] NAIVE_SQUARE(1+2)=" << a << " SAFE_SQUARE(1+2)=" << b << '\n';

    // 陷阱②：参数重复求值（副作用）
    int i = 0;
    int j = 0;
    // NAIVE_MAX(i++, j) 可能对 i++ 求值两次 → i 多增；现代代码用函数/模板
    [[maybe_unused]] const int m_macro = NAIVE_MAX(++i, j);  // ((++i) > (j) ? (++i) : (j)) → i 变成 2
    assert(i == 2);
    assert(m_macro == 2);
    i = 0;
    j = 1;
    [[maybe_unused]] const int m_fn = max_i(++i, j);  // ++i 只发生一次
    assert(i == 1);
    assert(m_fn == 1);  // max(1,1)==1；若 j 更大则返回 j，i 仍只 +1
    i = 0;
    j = 5;
    [[maybe_unused]] const int m_fn2 = max_i(++i, j);
    assert(i == 1 && m_fn2 == 5);
    std::cout << "[pitfall2] macro max double-evaluates; function max evaluates once\n";

    // 陷阱③：无作用域污染——#define max 100 会破坏 std::max / 变量 max（此处不真正定义，
    // 以免干扰本 TU；Windows 头常用 #define NOMINMAX 关掉 min/max 宏）。
    std::cout << "[pitfall3] object-like macros have no scope; prefer constexpr/inline/enum class\n";

    // 条件编译在阶段 4 删除死分支——编译器根本看不到被剔除的记号
#if 0
    static_assert(false, "this branch is discarded at phase 4");
#endif
    std::cout << "[advanced] #if 0 body never reaches the compiler\n";

    // __has_include：探测头是否存在（C++17）；配合 <version> / feature-test 是落地纪律
#if LEARN_HAS_VERSION_HEADER
    std::cout << "[advanced] <version> present; __cplusplus=" << __cplusplus << '\n';
#else
    std::cout << "[advanced] <version> not detected by __has_include\n";
#endif

    // -------------------------------------------------------------------------
    // §专家：与翻译模型的连接 + 实证命令
    // -------------------------------------------------------------------------
    // · 头文件本身不是翻译单元；被 N 个 .cpp include → 内容出现在 N 个 TU 中。
    // · 因此头里放非 inline 函数定义 → 多份定义 → multiple definition（见 ODR topic）。
    // · include guard / #pragma once 只防"同一 TU 内重复粘贴"，不防跨 TU 各一份。
    // · 阶段 4 结束后预处理指令全部消失；所以 .i 里搜不到 #include。
    // · 宏的现代边界：条件编译 + include guard；其余用 constexpr/inline/模板。
    //
    // 亲手看展开（在任意 hello.cpp 上）:
    //   g++ -std=c++23 -E hello.cpp -o hello.i
    //   cl /std:c++latest /P /C hello.cpp
    //   clang-cl /std:c++latest /E hello.cpp > hello.i
    // 打开 hello.i，搜索 main：你的代码在文件末尾，前面是 <iostream> 等展开。
    //
    // 搜索路径差异:
    //   #include <vector>     // 标准库
    //   #include "my_util.hpp" // 项目头
    // MSVC /I、Clang -I、CMake target_include_directories 都在改搜索路径，不改变"粘贴"语义。

    const std::string_view model = "include = copy-paste at phase 4; compiler sees one flat token stream per TU";
    assert(model.find("phase 4") != std::string_view::npos);
    assert(model.find("copy-paste") != std::string_view::npos);
    std::cout << "[expert] " << model << '\n';
    std::cout << "=== include_text_substitution: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/include_text_substitution", run>;

}  // namespace
