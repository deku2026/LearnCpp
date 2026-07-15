// Topic     : lambda 返回类型（推导与显式）
// Doc       : 第2部分-阶段3 · 步骤 5.1
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: 省略时从 return 推导；多 return 须一致；-> T 显式指定；
//       可与 auto 参数、trailing 组合。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [lambda_return_type] ===\n";

    // -------------------------------------------------------------------------
    // §入门：推导 vs 显式
    // -------------------------------------------------------------------------
    auto a = [] { return 42; };
    [[maybe_unused]] auto b = [](int x) { return x * 2; };
    auto c = [](int x) -> double { return x / 2.0; };
    assert(a() == 42);
    assert(b(21) == 42);
    assert(c(5) == 2.5);
    static_assert(std::is_same_v<decltype(a()), int>);
    static_assert(std::is_same_v<decltype(c(1)), double>);
    std::cout << "[intro] deduced vs trailing -> return type\n";

    // -------------------------------------------------------------------------
    // §进阶：多分支必须同一类型
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto abs_i = [](int x) {
        if (x < 0) {
            return -x;
        }
        return x;  // 同为 int
    };
    assert(abs_i(-3) == 3);

    // 强制统一：显式 -> std::string
    [[maybe_unused]] auto label = [](bool ok) -> std::string {
        if (ok) {
            return "yes";
        }
        return "no";
    };
    assert(label(true) == "yes");

    // 若一枝返回 int 一枝 double 且无 -> 指定，推导失败（编译错误）。
    std::cout << "[advanced] all return statements must agree (or write -> T)\n";

    // -------------------------------------------------------------------------
    // §专家：引用返回、void、复杂类型
    // -------------------------------------------------------------------------
    int storage = 10;
    auto get_ref = [&storage]() -> int& { return storage; };
    get_ref() = 99;
    assert(storage == 99);

    auto noop = [] { /* 无 return → void */ };
    static_assert(std::is_void_v<decltype(noop())>);

    [[maybe_unused]] auto either = [](bool left) -> std::variant<int, std::string> {
        if (left) {
            return 1;
        }
        return std::string{"two"};
    };
    assert(std::get<int>(either(true)) == 1);
    assert(std::get<std::string>(either(false)) == "two");

    std::cout << "[expert] -> T for ref/variant/void clarity; keep dangling out\n";
    std::cout << "=== lambda_return_type: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/lambda_return_type", run>;

}  // namespace
