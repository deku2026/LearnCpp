// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 生成器表达式(genex) / multi-config
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : generator_expressions
// Topic id : part4/section01/generator_expressions
//
// 要点: $<...> 在生成构建系统时求值, 非 configure 时。
//       常见: $<CONFIG>, $<BOOL:>, $<IF:>, $<BUILD_INTERFACE:>, $<TARGET_PROPERTY:>
// 仓库: CompilerWarnings.cmake 用 "$<$<COMPILE_LANGUAGE:CXX>:/permissive->"

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 极简 genex 求值器(教学用, 覆盖文档常见形态)
[[maybe_unused]] std::string eval_config(std::string_view config) {
    return std::string(config);  // $<CONFIG> → Debug/Release/...
}

bool eval_bool(std::string_view token) {
    // $<BOOL:x> — 空/0/FALSE/OFF/N/NO/IGNORE/NOTFOUND 为假
    if (token.empty()) {
        return false;
    }
    std::string u;
    u.reserve(token.size());
    for (char c : token) {
        u.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
    }
    return !(u == "0" || u == "FALSE" || u == "OFF" || u == "N" || u == "NO" || u == "IGNORE" || u == "NOTFOUND");
}

std::string eval_if(bool cond, std::string_view then_v, std::string_view else_v) {
    return std::string(cond ? then_v : else_v);
}

// $<CONFIG:Debug> 匹配
bool config_is(std::string_view actual, std::string_view want) {
    if (actual.size() != want.size()) {
        return false;
    }
    for (std::size_t i = 0; i < actual.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(actual[i])) != std::tolower(static_cast<unsigned char>(want[i]))) {
            return false;
        }
    }
    return true;
}

// 模拟: target_compile_options(... PRIVATE $<$<CONFIG:Debug>:-O0> $<$<CONFIG:Release>:-O3>)
std::string pick_opt_level(std::string_view config) {
    if (config_is(config, "Debug")) {
        return "-O0";
    }
    if (config_is(config, "Release")) {
        return "-O3";
    }
    if (config_is(config, "RelWithDebInfo")) {
        return "-O2";
    }
    return "-O1";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== generator_expressions ===\n";

    // --- 入门: $<CONFIG> ---
    assert(eval_config("Debug") == "Debug");
    assert(pick_opt_level("Debug") == "-O0");
    assert(pick_opt_level("Release") == "-O3");
    std::cout << "  Debug opts=" << pick_opt_level("Debug") << " Release=" << pick_opt_level("Release") << '\n';

    // --- 进阶: $<BOOL> + $<IF> ---
    assert(eval_bool("ON"));
    assert(eval_bool("1"));
    assert(!eval_bool("OFF"));
    assert(!eval_bool(""));
    const bool asan = eval_bool("ON");
    const auto flags = eval_if(asan, "-fsanitize=address", "");
    assert(flags == "-fsanitize=address");
    std::cout << "  BOOL/IF asan flags=\"" << flags << "\"\n";

    // --- 专家: COMPILE_LANGUAGE 门控(对照 CompilerWarnings.cmake) ---
    // "$<$<COMPILE_LANGUAGE:CXX>:/permissive->" 仅对 C++ 源加
    [[maybe_unused]] auto for_lang = [](std::string_view lang, std::string_view flag) {
        return lang == "CXX" ? std::string(flag) : std::string{};
    };
    assert(for_lang("CXX", "/permissive-") == "/permissive-");
    assert(for_lang("C", "/permissive-").empty());

    // multi-config 生成器: 同一套 genex 在 VS/Xcode 下按配置展开
    std::vector<std::string> configs{"Debug", "Release", "RelWithDebInfo"};
    for (const auto& c : configs) {
        std::cout << "  [" << c << "] " << pick_opt_level(c) << '\n';
    }
    assert(pick_opt_level("RelWithDebInfo") == "-O2");

    std::cout << "generator_expressions: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/generator_expressions", run>;

}  // namespace
