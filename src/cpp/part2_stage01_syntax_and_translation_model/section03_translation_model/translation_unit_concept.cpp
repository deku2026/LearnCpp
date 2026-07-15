// Topic     : 翻译单元（Translation Unit）概念
// Doc       : 第2部分-阶段1 · 步骤 2.5 / 5
// cppreference: https://en.cppreference.com/cpp/language/translation_phases
//               https://en.cppreference.com/cpp/language/definition
//
// 要点: TU = 源文件 + include − 条件编译剔除；头不是 TU；
//       N 个 .cpp ⇒ N 个 TU ⇒ N 个目标文件；条件编译如何改变 TU 边界。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// 模拟"头内容"被粘进本 TU
namespace tu_demo {
inline constexpr int from_header_like = 11;

#if defined(TU_DEMO_EXPERIMENTAL)
inline constexpr bool experimental = true;
#else
inline constexpr bool experimental = false;
#endif
}  // namespace tu_demo

// 本 .cpp 自身贡献的定义（与"头"共同组成一个 TU）
namespace tu_demo {
int source_only_counter = 0;
int bump() {
    return ++source_only_counter;
}
}  // namespace tu_demo

namespace {

struct ImaginaryBuild {
    std::vector<std::string> cpp_files;
    int translation_units() const { return static_cast<int>(cpp_files.size()); }
    // 每个 TU 产出一个目标文件（忽略 unity build / 模块等变体）
    int object_files() const { return translation_units(); }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [translation_unit_concept] ===\n";

    // -------------------------------------------------------------------------
    // §入门：定义
    // -------------------------------------------------------------------------
    // Translation unit =
    //   a source file
    //   + all headers included (recursively)
    //   − code removed by conditional compilation (#if / #ifdef / ...)
    // 预处理之后、送进编译器的那一整份记号流，就是一个 TU。

    assert(tu_demo::from_header_like == 11);
    assert(tu_demo::bump() == 1);
    assert(tu_demo::experimental == false);
    std::cout << "[intro] this .cpp + its includes (− dead #if branches) = one TU\n";

    ImaginaryBuild app;
    app.cpp_files = {"main.cpp", "net.cpp", "util.cpp"};
    assert(app.translation_units() == 3);
    assert(app.object_files() == 3);
    std::cout << "[intro] " << app.translation_units() << " .cpp files => " << app.object_files()
              << " object files (typical)\n";

    // -------------------------------------------------------------------------
    // §进阶：头不是 TU；条件编译裁剪 TU
    // -------------------------------------------------------------------------
    // common.hpp 被 3 个 .cpp include → 不是"第 4 个 TU"，而是 3 个 TU 各自含一份粘贴。
    // #if 0 / 未定义宏的分支在阶段 4 删除，根本不进入该 TU 的编译。
#if 0
    static_assert(false, "not part of this TU's compilation");
#endif
    std::cout << "[advanced] headers are not TUs; they are pasted into TUs\n";

    // Qt 对照: moc 生成 moc_foo.cpp —— 那是**额外的普通 TU**，仍走同一套编译链接。
    std::cout << "[advanced] moc_*.cpp is just another TU fed to the same pipeline\n";

    // -------------------------------------------------------------------------
    // §专家：TU 边界上的工程后果
    // -------------------------------------------------------------------------
    // 1) 可见性: 编译器编译 util.cpp 时看不到 net.cpp 里的静态函数体，只看得到声明。
    // 2) 并行: 多 TU 可并行编译（Ninja -j），链接是汇合点。
    // 3) 增量: 改一个头 → 所有 include 它的 TU 都要重编。
    // 4) ODR: 跨 TU 对同一实体的定义必须符合一处定义原则。
    // 5) 内部链接 / 匿名命名空间: 名字只在该 TU 内有效。
    // 6) C++20 模块改变组织方式，但传统 TU 模型仍是理解链接错误的基础。
    //
    // 实证:
    //   clang++ -c a.cpp -o a.o
    //   clang++ -c b.cpp -o b.o
    //   nm -C a.o b.o   # 分别查看两套符号表

    const std::string takeaway = "count TUs by source files after preprocessing, not by number of headers";
    assert(takeaway.find("headers") != std::string::npos);

    std::cout << "[expert] " << takeaway << '\n';
    std::cout << "=== translation_unit_concept: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/translation_unit_concept", run>;

}  // namespace
