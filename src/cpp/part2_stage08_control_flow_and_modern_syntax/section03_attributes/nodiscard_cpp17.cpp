// Topic    : [[nodiscard]]（C++17，C++20 可带原因字符串）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 6.1
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : nodiscard_cpp17
// Topic id : part2/stage08/section03/nodiscard_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/attributes/nodiscard
//            提案 P0189；ISO [dcl.attr.nodiscard]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

// 返回值表示成败：忽略几乎必是 bug。
[[nodiscard]] bool try_connect(const char* host) {
    return host != nullptr && host[0] != '\0';
}

// C++20：可附带原因，警告信息更可读。
[[nodiscard("ignoring the error code may leak the handle")]]
int open_resource(const char* path) {
    if (path == nullptr || path[0] == '\0') {
        return -1;
    }
    return 7;  // 假想句柄
}

// 标在类型上：凡返回该类型的函数都“不可丢弃返回值”。
struct [[nodiscard]] ErrorCode {
    int code{};
    explicit operator bool() const { return code == 0; }
};

ErrorCode write_all(const std::string& /*payload*/) {
    return ErrorCode{0};
}

// 查询 API：empty 不是 clear；标 nodiscard 防止写成 if (v.empty); 的笔误之外的“调用了却没用”。
[[maybe_unused]] [[nodiscard]] bool is_empty(const std::vector<int>& v) {
    return v.empty();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [nodiscard_cpp17] 入门：用返回值 / 显式丢弃 ===\n";
    {
        const bool ok = try_connect("example.com");
        assert(ok);
        std::cout << "try_connect ok=" << std::boolalpha << ok << '\n';

        // 若写成 try_connect("x"); 且开启足够告警，编译器应警告 nodiscard。
        // 教学可运行路径：必须使用或显式 (void) 丢弃。
        [[maybe_unused]] const bool no_host = try_connect("");
        assert(!no_host);

        // 显式丢弃：告诉读者“我知道有返回值，故意不要”。
        (void)try_connect("intentionally-ignored-demo");
        std::cout << "explicit (void) cast documents intentional discard\n";
    }

    std::cout << "=== 进阶：原因字符串、类型级 nodiscard、链式检查 ===\n";
    {
        const int h = open_resource("/tmp/x");
        assert(h == 7);
        std::cout << "handle=" << h << '\n';
        assert(open_resource("") == -1);

        [[maybe_unused]] ErrorCode ec = write_all("payload");
        assert(static_cast<bool>(ec));
        // ErrorCode 带 [[nodiscard]]：丢弃 write_all(...) 的返回值也会警告。

        std::vector<int> v{1};
        assert(!is_empty(v));
        v.clear();
        assert(is_empty(v));

        // 工厂/异步：std::async 等标准 API 大量使用 nodiscard，
        // 防止“启动了却没拿 future，析构时同步等待”的意外。
        std::cout << "type-level [[nodiscard]] covers all returns of that type\n";
    }

    std::cout << "=== 专家：何时标注、与 [[maybe_unused]] 分工、工程约定 ===\n";
    {
        // 适合 nodiscard 的返回值：
        // - 错误码 / expected / optional（忽略=吞错）
        // - 新对象 / 句柄（忽略=泄漏或白算）
        // - 纯查询（empty/size 的“以为有副作用”类 API）
        // 不适合：本身以副作用为主、返回值仅偶尔关心的过程式调用
        // （否则警告噪声过高）。

        // 与 maybe_unused：nodiscard 管“调用结果”；maybe_unused 管“实体可能未 ODR 使用”。
        // 需要丢弃 nodiscard 结果时用 static_cast<void>(...) 或赋值给 maybe_unused 变量。

        [[maybe_unused]] ErrorCode ignored = write_all("x");
        // 若必须忽略： (void)write_all("x"); 在开启 nodiscard 警告的工具链上仍可能告警，
        // 视实现而定；赋值给变量通常算“使用”。

        assert(static_cast<bool>(write_all("y")));
        std::cout << "mark status-returning APIs; keep side-effect APIs quiet\n";
        std::cout << "verify discard warning with -Wunused-result / MSVC C4834\n";
    }

    std::cout << "[nodiscard_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/nodiscard_cpp17", run>;

}  // namespace
