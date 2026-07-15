// Topic    : switch 语句（多路分支、穿透、枚举）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 1 / 6.2
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : switch_statement
// Topic id : part2/stage08/section01/switch_statement
// Refs     : https://en.cppreference.com/w/cpp/language/switch
//            ISO [stmt.switch]；[[fallthrough]] 见 section03

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

enum class HttpStatus : int {
    Ok = 200,
    Created = 201,
    NotFound = 404,
    ServerError = 500,
};

// 故意演示 fallthrough：2xx 成功族共享一段逻辑。
[[maybe_unused]] std::string classify_status(HttpStatus s) {
    switch (s) {
        case HttpStatus::Ok:
        case HttpStatus::Created:
            return "success";
        case HttpStatus::NotFound:
            return "client_error";
        case HttpStatus::ServerError:
            return "server_error";
    }
    return "unknown";  // 枚举穷尽后通常不可达；保留防御
}

[[maybe_unused]] int days_in_month(int month, bool leap) {
    // 经典 case 穿透：多个月共享天数。
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            return leap ? 29 : 28;
        default:
            return -1;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [switch_statement] 入门：整型多路分支 + break ===\n";
    {
        int code = 2;
        const char* name = "unset";
        switch (code) {
            case 0:
                name = "zero";
                break;
            case 1:
                name = "one";
                break;
            case 2:
                name = "two";
                break;
            default:
                name = "other";
                break;
        }
        assert(std::string{name} == "two");
        std::cout << "code=2 -> " << name << '\n';
    }

    std::cout << "=== 进阶：穿透、枚举、case 作用域 ===\n";
    {
        assert(days_in_month(1, false) == 31);
        assert(days_in_month(4, false) == 30);
        assert(days_in_month(2, false) == 28);
        assert(days_in_month(2, true) == 29);
        assert(days_in_month(13, false) == -1);
        std::cout << "days_in_month table ok\n";

        assert(classify_status(HttpStatus::Ok) == "success");
        assert(classify_status(HttpStatus::Created) == "success");
        assert(classify_status(HttpStatus::NotFound) == "client_error");
        std::cout << "enum class switch ok\n";

        // case 标签不引入作用域：若在 case 内声明带初始化的变量，
        // 需要用复合语句包住，避免跳过初始化。
        int mode = 1;
        int result = 0;
        switch (mode) {
            case 1: {
                int local = 10;  // 复合语句保护
                result = local * 2;
                break;
            }
            case 2: {
                int local = 3;
                result = local;
                break;
            }
            default:
                result = -1;
                break;
        }
        assert(result == 20);
        std::cout << "case compound scope: result=" << result << '\n';
    }

    std::cout << "=== 专家：穿透陷阱、整型/枚举限制、与 if 的取舍 ===\n";
    {
        // ⚠️ 忘写 break 是经典 bug：会“穿透”到下一 case。
        int x = 1;
        int acc = 0;
        switch (x) {
            case 1:
                acc += 10;
                // 故意无 break —— 教学用，生产里应写 [[fallthrough]] 或 break
            case 2:
                acc += 1;
                break;
            default:
                acc = -1;
                break;
        }
        assert(acc == 11);  // 1 → 10，再穿透加 1
        std::cout << "implicit fallthrough acc=" << acc << " (prefer [[fallthrough]] when intentional)\n";

        // switch 条件须是整型或枚举（或可整型转换）；不能直接 switch 字符串。
        // C++17 起可写 switch (init; cond)，见 section02。
        // C++20 起 case 可标 [[likely]]/[[unlikely]]，见 section03。

        // 工程取舍：
        // - 密集整型标签 → switch 常被编译为跳转表，可预测性好
        // - 稀疏标签或复杂谓词 → if 链 / 表驱动更好
        // - 开放类型集 → variant+visit（section05）
        std::cout << "switch: integral/enum labels; strings need map/if\n";
    }

    std::cout << "[switch_statement] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/switch_statement", run>;

}  // namespace
