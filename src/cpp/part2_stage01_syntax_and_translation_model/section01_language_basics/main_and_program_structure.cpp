// Topic     : main 与程序结构
// Doc       : 第2部分-阶段1 · 步骤 1
// cppreference: https://en.cppreference.com/cpp/language/main_function
// ISO       : [basic.start.main]
//
// 要点: 托管环境下的入口约定、合法签名、隐式 return 0、启动/退出序列、
//       命令行参数布局；本 topic 在 learn_cpp 的 run() 里对照讲解（真实 main
//       由工程入口提供，不能在此重定义）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 模拟"进程启动前已经发生的静态初始化"——全局/命名空间作用域对象
// 在 main 被调用之前完成初始化（本文件的静态对象同理，在 run 之前构造）。
struct StartupProbe {
    int stamp = 0;
    StartupProbe() : stamp(1001) {
        // 构造发生在该 TU 的动态初始化阶段；对宿主程序而言早于 main。
    }
};

const StartupProbe g_probe{};

// 合法托管签名的"教学替身"：真正的 main 只能有一个，这里用同签名函数演示
// argc/argv 约定，而不是声明第二个 main。
int hosted_entry_like(int argc, char** argv) {
    // 隐式 return 0 只属于真正的 main；普通函数必须显式返回。
    if (argc < 1) {
        return EXIT_FAILURE;
    }
    assert(argv != nullptr);
    // argv[argc] 在标准中保证为 null（当 argc 由运行时按规范传入时）。
    return EXIT_SUCCESS;
}

int run(int argc, char** argv) {
    std::cout << "=== [main_and_program_structure] ===\n";

    // -------------------------------------------------------------------------
    // §入门：托管环境入口心智
    // -------------------------------------------------------------------------
    // 标准可移植签名仅两种:
    //   int main()
    //   int main(int argc, char* argv[])   // 或 char** argv
    // 返回类型必须是 int；void main / auto main 非法。
    // 落到 main 末尾而无 return 时，等价于 return 0（仅 main 豁免）。
    //
    // 启动序列（托管）概要:
    //   OS 加载 → CRT 初始化 → 静态存储期非局部对象初始化 → 调用 main
    //   → main 返回 → 等价于以该值调用 std::exit → 销毁静态对象、刷流、交还退出码

    assert(g_probe.stamp == 1001);
    std::cout << "[intro] static object constructed before entry path; stamp=" << g_probe.stamp << '\n';

    // -------------------------------------------------------------------------
    // §进阶：argc / argv 布局 + 退出码语义
    // -------------------------------------------------------------------------
    // learn_cpp 调用形态: learn_cpp <topic-id> [extra args...]
    // 因此这里的 argc/argv 是宿主 main 转发给 topic 的，不是 OS 原始 argv 的"程序名"，
    // 但仍可验证"参数计数 + 字符串数组"的用法。
    std::cout << "[args] argc=" << argc << '\n';
    for (int i = 0; i < argc; ++i) {
        assert(argv[i] != nullptr);
        std::cout << "  argv[" << i << "] = \"" << argv[i] << "\"\n";
    }

    // 可移植成功/失败码（比裸 0/1 更表意）。
    // 标准保证：return 0 与 return EXIT_SUCCESS 都表示成功；EXIT_SUCCESS 的数值实现定义，
    // 但在所有主流托管实现上为 0——这里用运行期断言而非「==0 || true」空壳。
    const int ok = hosted_entry_like(argc > 0 ? argc : 1, argc > 0 ? argv : nullptr);
    assert(ok == EXIT_SUCCESS);
    assert(EXIT_FAILURE != EXIT_SUCCESS);  // 失败码必须可与成功区分
    // 其它非 0 退出码含义实现定义（shell 的 $? / %ERRORLEVEL%）
    std::cout << "[exit] EXIT_SUCCESS=" << EXIT_SUCCESS << " EXIT_FAILURE=" << EXIT_FAILURE << '\n';

    // 把 argv 拷成 string_view 向量：演示"可遍历至 argc-1"，不读 argv[argc]
    std::vector<std::string_view> views;
    views.reserve(static_cast<std::size_t>(argc > 0 ? argc : 0));
    for (int i = 0; i < argc; ++i) {
        views.emplace_back(argv[i]);
    }
    assert(static_cast<int>(views.size()) == (argc > 0 ? argc : 0));

    // -------------------------------------------------------------------------
    // §专家：main 的特殊限制（为何不能当普通函数用）
    // -------------------------------------------------------------------------
    // 1) 不可重载：全局命名空间中入口名唯一。
    // 2) 不可 inline / static / constexpr / =delete / extern "C"。
    // 3) 不可取地址、不可在程序里递归调用 main（ill-formed）。
    // 4) C++14 起明确禁止 auto main()。
    // 5) 实现定义扩展 int main(int, char**, char** envp) 不保证可移植。
    // 6) freestanding 环境入口名/类型可实现定义——不必叫 main。
    //
    // 对照：普通非 void 函数落到末尾不 return 是 UB；main 是唯一"隐式 return 0"的函数。
    //
    // 工具验证（在独立 hello.cpp 上操作，勿对本 learn_cpp 目标乱改）:
    //   cl /std:c++latest /W4 /EHsc /Zc:__cplusplus hello.cpp
    //   clang-cl /std:c++latest /permissive- /Zc:__cplusplus /EHsc /W4 hello.cpp
    //   省略 return 的 main → echo %ERRORLEVEL% 应为 0
    //
    // godbolt: 观察 main 末尾把返回寄存器置 0 的指令序列。

    std::cout << "[expert] main is entry-only: no overload, no &main, no auto main,\n"
              << "         implicit return 0 only for real main; ordinary fns need return.\n";

    // 小断言：字符串字面量程序退出语义在文档中的对应关系
    const std::string note = "return 0 / EXIT_SUCCESS => success";
    assert(note.find("EXIT_SUCCESS") != std::string::npos);

    std::cout << "=== main_and_program_structure: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/main_and_program_structure", run>;

}  // namespace
