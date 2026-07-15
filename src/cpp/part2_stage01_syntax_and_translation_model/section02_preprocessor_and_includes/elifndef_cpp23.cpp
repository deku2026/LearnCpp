// Topic     : #elifndef（C++23）
// Doc       : 第2部分-阶段1 · 步骤 3.5
// cppreference: https://en.cppreference.com/cpp/preprocessor/conditional
// 提案      : https://wg21.link/P2334
//
// 要点: #elifndef ID ≡ #elif !defined(ID)；与 #ifndef / #elif !defined 对称；
//       用于"默认路径 / 缺省特性"配置；可移植兜底。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

// 场景：日志后端。若用户没强制选择，则走默认。
// #define LEARN_LOG_FORCE_JSON 1
// #define LEARN_LOG_FORCE_PLAIN 1
#define LEARN_LOG_ENABLE_COLOR 1

#if defined(LEARN_LOG_FORCE_JSON)
inline constexpr const char* k_log_mode = "json-forced";
#elifndef LEARN_LOG_FORCE_PLAIN
// 未强制 plain → 再看颜色开关（演示 #elifndef 进入后的内层逻辑）
#if defined(LEARN_LOG_ENABLE_COLOR)
inline constexpr const char* k_log_mode = "text-color";
#else
inline constexpr const char* k_log_mode = "text-mono";
#endif
#else
inline constexpr const char* k_log_mode = "plain-forced";
#endif

// 可移植等价链
#if defined(LEARN_LOG_FORCE_JSON)
inline constexpr int k_mode_code = 1;
#elif !defined(LEARN_LOG_FORCE_PLAIN)
#if defined(LEARN_LOG_ENABLE_COLOR)
inline constexpr int k_mode_code = 2;
#else
inline constexpr int k_mode_code = 3;
#endif
#else
inline constexpr int k_mode_code = 4;
#endif

// 另一独立例子：缺省启用安全检查
// #define LEARN_DISABLE_BOUNDS_CHECKS 1
#if defined(LEARN_DISABLE_BOUNDS_CHECKS)
inline constexpr bool k_bounds_checks = false;
#elifndef LEARN_DISABLE_BOUNDS_CHECKS
inline constexpr bool k_bounds_checks = true;
#else
// 逻辑上不可达：#elifndef 已覆盖"未定义"情况
inline constexpr bool k_bounds_checks = true;
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [elifndef_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：#elifndef = #elif !defined
    // -------------------------------------------------------------------------
    assert(std::string{k_log_mode} == "text-color");
    assert(k_mode_code == 2);
    assert(k_bounds_checks);
    std::cout << "[intro] log_mode=" << k_log_mode << " bounds_checks=" << std::boolalpha << k_bounds_checks << '\n';

    // -------------------------------------------------------------------------
    // §进阶：何时用 #elifndef 而不是 #else
    // -------------------------------------------------------------------------
    // 当 elif 链需要表达"若尚未定义某开关"，且后面还有 #else/#elifdef 其它分支时，
    // #elifndef 比嵌套 #if !defined 更扁平、可读。
    // 若只是简单二分，#ifdef / #else / #endif 往往够用。
    std::cout << "[advanced] #elifndef shines in multi-branch feature ladders\n";

    // 对照表（记忆）:
    //   #ifdef X     ~  #if defined(X)
    //   #ifndef X    ~  #if !defined(X)
    //   #elifdef X   ~  #elif defined(X)     (C++23)
    //   #elifndef X  ~  #elif !defined(X)    (C++23)

    // -------------------------------------------------------------------------
    // §专家：与 #elifdef 共同的落地风险
    // -------------------------------------------------------------------------
    // · 老预处理器不认识 #elifndef → 可能把整行当未知指令，条件链错乱。
    // · 兜底: #elif !defined(ID)
    // · 宏名拼写错误时 #elifndef TYPO 会"以为没定义"而误入分支——加集成测试覆盖配置矩阵。
    // · 被跳过分支中的 #define 不生效；依赖宏副作用的代码要格外小心顺序。
    //
    // 实验建议: 定义 LEARN_LOG_FORCE_PLAIN 后应得到 plain-forced / code 4。

#if defined(LEARN_LOG_FORCE_PLAIN)
    static_assert(false, "force plain not set in default demo");
#elifndef LEARN_LOG_FORCE_JSON
    static_assert(true, "expected path: neither json force nor (in this arm) ...");
#endif

    std::cout << "[expert] portable fallback: #elif !defined(NAME)\n";
    std::cout << "=== elifndef_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/elifndef_cpp23", run>;

}  // namespace
