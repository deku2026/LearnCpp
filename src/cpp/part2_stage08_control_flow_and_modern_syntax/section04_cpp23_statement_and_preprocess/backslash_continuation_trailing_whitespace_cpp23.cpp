// Topic    : 行拼接前修剪 \ 后尾随空白（P2223, C++23）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 9.5
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : backslash_continuation_trailing_whitespace_cpp23
// Topic id : part2/stage08/section04/backslash_continuation_trailing_whitespace_cpp23
// Refs     : https://en.cppreference.com/w/cpp/language/translation_phases
//            提案 P2223R2；相位 2 行拼接

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

// 多行宏依赖行末 \ 续行。C++23 前：\ 后若有不可见尾随空格，续行失败 → 离奇编译错误。
// C++23：拼接前先丢掉 \ 后空白，宏更稳健。
//
// 注意：源文件里我们仍写“干净的 \”（无尾随空白），以保证所有阶段都可编译；
// 本 topic 用可运行断言说明续行语义，并用注释/字符串说明 P2223 的坑与修复。

#define LEARN_STAGE08_ADD3(a, b, c) ((a) + (b) + (c))

#define LEARN_STAGE08_MAX(a, b) (((a) > (b)) ? (a) : (b))

// 字符串字面量也可物理续行（少用；原始字符串更清晰）
[[maybe_unused]] const char* continued_message() {
    return "hello "
           "world";  // 相邻字符串拼接（相位 6），不是反斜杠续行
}

const char* backslash_continued() {
    return "line1 \
line2";  // 反斜杠续行：两行源码 → 一个字符串
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [backslash_continuation_trailing_whitespace_cpp23] 入门 ===\n";
    {
        assert(LEARN_STAGE08_ADD3(1, 2, 3) == 6);
        assert(LEARN_STAGE08_MAX(4, 9) == 9);
        std::cout << "multi-line macros via \\ continuation ok\n";
    }

    std::cout << "=== 进阶：续行 vs 相邻字符串字面量 ===\n";
    {
        assert(std::string{continued_message()} == "hello world");
        // 反斜杠续行保留换行后的内容；注意缩进会进字符串
        const std::string s = backslash_continued();
        assert(s.find("line1") != std::string::npos);
        assert(s.find("line2") != std::string::npos);
        std::cout << "backslash-continued string length=" << s.size() << '\n';

        // 翻译相位（简表）：
        // 相位 2：物理源行用 \ 拼接成逻辑行（P2223 在此修剪 \ 后空白）
        // 相位 3：记号化
        // 相位 4：预处理
        // 相位 6：相邻字符串字面量合并
    }

    std::cout << "=== 专家：P2223 修了什么、如何自查 ===\n";
    {
        // 历史坑：编辑器/邮件/格式化在 \ 后留下 space/tab，
        // 续行失效 → “宏第一行突然结束”、下一行变成裸代码 → 报错位置离谱。
        // C++23：实现必须在拼接前忽略 \ 后的横空白，坑被标准抹平。
        //
        // 自查旧代码：
        // - 开“显示空白”看 \ 后是否有点/箭头
        // - clang-format / cat -A / 十六进制查看
        // - 尽量少用手写多行宏；用 inline/constexpr/变参模板替代
        //
        // 本仓库源文件保持 \ 后无空白，C++17 工具链也能编过；
        // 特性价值是“防御性”——脏源文件在 C++23 下不再因此炸裂。

        [[maybe_unused]] const int v = LEARN_STAGE08_ADD3(10, 20, 30);
        assert(v == 60);
        std::cout << "P2223: trim whitespace after \\ before line splicing\n";
        std::cout << "prefer functions over fragile multi-line macros\n";
    }

    std::cout << "[backslash_continuation_trailing_whitespace_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage08/section04/backslash_continuation_trailing_whitespace_cpp23", run>;

}  // namespace
