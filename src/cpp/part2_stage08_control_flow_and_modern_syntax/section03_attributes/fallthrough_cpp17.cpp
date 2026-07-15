// Topic    : [[fallthrough]]（C++17）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 6.2
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : fallthrough_cpp17
// Topic id : part2/stage08/section03/fallthrough_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/attributes/fallthrough
//            ISO [dcl.attr.fallthrough]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

// 命令分派：部分 case 共享收尾逻辑，故意穿透。
std::string handle_command(int code) {
    std::string log;
    switch (code) {
        case 1:
            log += "prepare;";
            [[fallthrough]];  // 故意进入 case 2 的公共路径
        case 2:
            log += "run_common;";
            break;
        case 3:
            log += "special;";
            break;
        default:
            log += "default;";
            break;
    }
    return log;
}

// 字符分类：大小写共用。
[[maybe_unused]] bool is_hex_digit(char c) {
    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
        case 'a':
        case 'A':
            [[fallthrough]];
        case 'b':
        case 'B':
        case 'c':
        case 'C':
        case 'd':
        case 'D':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
            return true;
        default:
            return false;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [fallthrough_cpp17] 入门：标记故意穿透 ===\n";
    {
        assert(handle_command(1) == "prepare;run_common;");
        assert(handle_command(2) == "run_common;");
        assert(handle_command(3) == "special;");
        assert(handle_command(9) == "default;");
        std::cout << "case1 -> " << handle_command(1) << '\n';
    }

    std::cout << "=== 进阶：语法位置、与空 case 列表 ===\n";
    {
        // [[fallthrough]]; 必须落在“空语句”上，位于 case 可执行语句之间，
        // 且下一句应是 case/default 标签（实现按 -Wimplicit-fallthrough 识别）。
        int x = 0;
        [[maybe_unused]] int path = 0;
        switch (x) {
            case 0:
                path += 1;
                [[fallthrough]];
            case 1:
                path += 10;
                break;
            default:
                path = -1;
                break;
        }
        assert(path == 11);

        // 连续 case 标签共享体时，无需 fallthrough 属性（没有“可执行语句后落入”）。
        assert(is_hex_digit('9'));
        assert(is_hex_digit('a'));
        assert(is_hex_digit('F'));
        assert(!is_hex_digit('g'));
        std::cout << "hex classifier ok\n";
    }

    std::cout << "=== 专家：警告模型、误用、与 break 纪律 ===\n";
    {
        // 意图：
        // - 默认穿透是 C switch 历史包袱；现代编译器对“语句后无 break 落入下一 case”告警
        // - [[fallthrough]] 显式关闭该告警，留下可读的“我故意”文档
        //
        // 误用：
        // - 写在最后 case 末尾无意义
        // - 不能替代 break：属性不改变控制流，只影响诊断
        // - 属性后不能再夹杂其它语句再落入（否则仍可能告警/病式，视实现）

        [[maybe_unused]] int acc = 0;
        switch (2) {
            case 2:
                acc = 2;
                break;  // 正常终止，不需要 fallthrough
            case 3:
                acc = 3;
                break;
            default:
                acc = 0;
                break;
        }
        assert(acc == 2);

        // 工程：能把共享逻辑提成函数就别穿透；穿透仅适合极短公共尾声。
        // 开启 -Wimplicit-fallthrough（GCC/Clang）或等效 MSVC 分析更安全。
        std::cout << "[[fallthrough]] documents intent; prefer extract function\n";
    }

    std::cout << "[fallthrough_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/fallthrough_cpp17", run>;

}  // namespace
