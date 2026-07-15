// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.1–4.1.2 现代 CMake target-based
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : cmake_target_basics
// Topic id : part4/section01/cmake_target_basics
//
// 要点: 现代 CMake 围绕 target 组织属性(include/flags/libs),
//       告别 set(CMAKE_CXX_FLAGS ...) 全局污染。
// 仓库对照: 根 CMakeLists.txt 的 add_executable(learn_cpp) +
//           target_compile_features / target_include_directories;
//           cmake/CompilerWarnings.cmake 的 learncpp_apply_warning_settings(target).

#include "learn/topic_registry.hpp"

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 用 C++ 小模型模拟「target 属性袋」——对应 CMake 的 target_compile_options 等。
struct Target {
    std::string name;
    std::set<std::string> compile_options;  // PRIVATE 语义: 仅自身
    std::set<std::string> link_libs;
    std::string cxx_standard{"23"};
};

// ❌ 旧式全局: 一条 flags 影响一切
struct GlobalFlags {
    std::string cxx_flags;
};

void apply_global_wall(GlobalFlags& g) {
    if (!g.cxx_flags.empty()) {
        g.cxx_flags += ' ';
    }
    g.cxx_flags += "-Wall";
}

// ✅ 现代 target-based: 只改指定 target
void target_compile_options(Target& t, std::string_view opt) {
    t.compile_options.emplace(opt);
}

void target_link_libraries(Target& t, std::string_view lib) {
    t.link_libs.emplace(lib);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== cmake_target_basics: global vs target-based ===\n";

    // --- 入门: 全局污染 ---
    GlobalFlags g{};
    apply_global_wall(g);
    // 第二个 target 无法选择不要 -Wall —— 已全局生效
    assert(g.cxx_flags.find("-Wall") != std::string::npos);
    std::cout << "  global CMAKE_CXX_FLAGS-like: \"" << g.cxx_flags << "\"\n";

    // --- 进阶: 多 target 各自属性 ---
    Target core{};
    core.name = "core";
    Target myapp{};
    myapp.name = "myapp";
    target_compile_options(core, "-Wall");
    target_compile_options(core, "-Wextra");
    core.cxx_standard = "23";  // target_compile_features(... cxx_std_23)
    target_link_libraries(myapp, "core");
    // myapp 没有 -Wall, 除非显式加
    assert(core.compile_options.count("-Wall") == 1);
    assert(myapp.compile_options.count("-Wall") == 0);
    assert(myapp.link_libs.count("core") == 1);
    assert(core.cxx_standard == "23");

    std::cout << "  core opts:";
    for (const auto& o : core.compile_options) {
        std::cout << ' ' << o;
    }
    std::cout << "\n  myapp links:";
    for (const auto& l : myapp.link_libs) {
        std::cout << ' ' << l;
    }
    std::cout << '\n';

    // --- 专家: 仓库真实映射 ---
    // CMakeLists.txt:
    //   add_executable(learn_cpp ${LEARNCPP_SOURCES})
    //   target_compile_features(learn_cpp PRIVATE cxx_std_23)
    //   target_include_directories(learn_cpp PRIVATE include/)
    //   learncpp_apply_warning_settings(learn_cpp)  // 仅此 target
    //   learncpp_enable_sanitizers(learn_cpp)
    std::map<std::string, Target> graph;
    graph["learn_cpp"] = Target{};
    graph["learn_cpp"].name = "learn_cpp";
    target_compile_options(graph["learn_cpp"], "cxx_std_23");
    target_compile_options(graph["learn_cpp"], "PRIVATE_include");
    assert(graph["learn_cpp"].compile_options.size() == 2);
    std::cout << "  learn_cpp property bag size=" << graph["learn_cpp"].compile_options.size() << '\n';

    // 多库骨架: core 库 + app —— 文档 4.1.2
    Target engine{};
    engine.name = "core_engine";
    Target app{};
    app.name = "myapp";
    target_link_libraries(app, "core_engine");
    assert(app.link_libs.contains("core_engine"));

    std::cout << "cmake_target_basics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/cmake_target_basics", run>;

}  // namespace
