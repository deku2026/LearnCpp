// Topic     : visit + overloaded 惯用法 / variant 状态机 (验收点)
// Doc       : 第2部分-阶段10 · 步骤 9.2
// cppreference: https://en.cppreference.com/cpp/utility/variant/visit
//
// 要点: template overloaded : Ts...; visit 穷尽处理; 状态机示例。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

namespace {

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

struct Idle {};
struct Running {
    int progress;
};
struct Done {
    int result;
};
using State = std::variant<Idle, Running, Done>;

State next(State s) {
    return std::visit(
        overloaded{
            [](Idle) -> State { return Running{0}; },
            [](Running r) -> State { return r.progress < 100 ? State{Running{r.progress + 50}} : State{Done{42}}; },
            [](Done d) -> State { return d; },
        },
        s);
}

std::string describe(const State& s) {
    return std::visit(overloaded{
                          [](Idle) { return std::string{"idle"}; },
                          [](Running r) { return std::string{"running "} + std::to_string(r.progress) + "%"; },
                          [](Done d) { return std::string{"done "} + std::to_string(d.result); },
                      },
                      s);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [variant_visit_overload_idiom] ===\n";

    State s = Idle{};
    assert(describe(s) == "idle");

    s = next(s);
    assert(describe(s) == "running 0%");
    s = next(s);
    assert(describe(s) == "running 50%");
    s = next(s);
    assert(describe(s) == "running 100%");
    s = next(s);
    assert(describe(s) == "done 42");
    s = next(s);
    assert(describe(s) == "done 42");  // 终态

    std::cout << "[state machine] Idle→Running→Done via visit OK\n";
    std::cout << "variant_visit_overload_idiom: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/variant_visit_overload_idiom", run>;

}  // namespace
