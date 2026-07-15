// Topic    : std::variant + std::visit + overloaded（模式匹配替代）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 10
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section05_pattern_matching_alternatives
// Item     : variant_visit_overload
// Topic id : part2/stage08/section05/variant_visit_overload
// Refs     : https://en.cppreference.com/w/cpp/utility/variant
//            https://en.cppreference.com/w/cpp/utility/variant/visit
//            提案 P0088；模式匹配方向 P2688（C++26）

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace {

// overloaded 惯用法：合并多个 lambda 成一个重载集
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

// C++20 起聚合基类可推导，显式推导指引可省；保留以兼容教学说明
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// 简单状态机：类型即状态
struct Idle {};
struct Running {
    int progress{};
};
struct Done {
    std::string result;
};

using State = std::variant<Idle, Running, Done>;

std::string describe(const State& st) {
    return std::visit(overloaded{
                          [](const Idle&) { return std::string{"idle"}; },
                          [](const Running& r) { return std::string{"running:"} + std::to_string(r.progress); },
                          [](const Done& d) { return std::string{"done:"} + d.result; },
                      },
                      st);
}

State step(State st) {
    return std::visit(overloaded{
                          [](Idle) -> State { return Running{0}; },
                          [](Running r) -> State {
                              r.progress += 50;
                              if (r.progress >= 100) {
                                  return Done{"ok"};
                              }
                              return r;
                          },
                          [](Done d) -> State { return d; },
                      },
                      st);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [variant_visit_overload] 入门：类型安全联合 + visit ===\n";
    {
        std::variant<int, double, std::string> v = 42;
        assert(v.index() == 0);
        assert(std::get<int>(v) == 42);

        v = 3.14;
        assert(std::holds_alternative<double>(v));
        assert(std::get<double>(v) == 3.14);

        v = std::string{"hello"};
        assert(std::get<std::string>(v) == "hello");

        // visit + overloaded：按当前持有类型分派（最接近模式匹配）
        std::string kind;
        std::visit(overloaded{
                       [&](int) { kind = "int"; },
                       [&](double) { kind = "double"; },
                       [&](const std::string&) { kind = "string"; },
                   },
                   v);
        assert(kind == "string");
        std::cout << "active alternative kind=" << kind << '\n';
    }

    std::cout << "=== 进阶：三种 visitor 风格 + 状态机 ===\n";
    {
        std::variant<int, std::string> u = 7;

        // 1) 泛型 lambda：所有类型同一套处理
        std::string generic;
        std::visit(
            [&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    generic = "i:" + std::to_string(arg);
                } else {
                    generic = "s:" + arg;
                }
            },
            u);
        assert(generic == "i:7");

        // 2) if constexpr 分派（上） / 3) overloaded（下，首选清晰）
        u = std::string{"z"};
        std::string via_overloaded;
        std::visit(overloaded{
                       [&](int i) { via_overloaded = std::to_string(i); },
                       [&](const std::string& s) { via_overloaded = s; },
                   },
                   u);
        assert(via_overloaded == "z");

        // 状态机
        State st = Idle{};
        assert(describe(st) == "idle");
        st = step(std::move(st));
        assert(describe(st) == "running:0");
        st = step(std::move(st));
        assert(describe(st) == "running:50");
        st = step(std::move(st));
        assert(describe(st) == "done:ok");
        std::cout << "state machine -> " << describe(st) << '\n';
    }

    std::cout << "=== 专家：穷尽检查、错误取值、与 QVariant / C++26 对照 ===\n";
    {
        // visit 要求 visitor 能接受每一种 alternative —— 漏写会编译失败（穷尽性）。
        // std::get<T> 取错类型 → 抛 bad_variant_access；先 holds_alternative 或用 get_if。

        std::variant<int, std::string> v = 1;
        if (auto* p = std::get_if<int>(&v)) {
            (void)p;
            assert(*p == 1);
        } else {
            assert(false);
        }
        assert(std::get_if<std::string>(&v) == nullptr);

        try {
            (void)std::get<std::string>(v);
            assert(false && "should throw");
        } catch (const std::bad_variant_access&) {
            std::cout << "get<wrong> throws bad_variant_access\n";
        }

        // overloaded 原理：继承各 lambda，using 包展开引入 operator() 重载集。
        //
        // 🔶 Qt QVariant：运行期类型擦除 + qvariant_cast；灵活但无编译期穷尽。
        // std::variant：编译期封闭类型集，适合 AST/协议/状态机。
        //
        // C++26 方向：语言级 pattern matching（P2688 等）——目前生产以 visit 为准。

        std::cout << "prefer visit+overloaded for closed sum types\n";
    }

    std::cout << "[variant_visit_overload] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section05/variant_visit_overload", run>;

}  // namespace
