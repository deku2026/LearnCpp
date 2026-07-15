// LearnCpp topic
// Doc      : 第6部分-支线G · G modules / import std
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : modules_import_std_impact_on_compile_model_cpp23
// Topic id : part6/g/section02/modules_import_std_impact_on_compile_model_cpp23
//
// 要点: 模块改变编译模型 — BMI/IFC 替代头文件重复解析;
//       import std (C++23) 提供标准库模块。本 topic 仍用 #include 兼容构建。
// 参考: [module.unit] P2465 import std

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G modules / import std impact (C++23) ===\n";

    std::vector<std::string> v{"a", "b"};
    assert(v.size() == 2);

    std::cout << "  header model: textual inclusion, macros leak, ODR risk\n";
    std::cout << "  modules: named ownership, faster rebuilds, fewer macros\n";
    std::cout << "  import std; // C++23 — one import for the standard library\n";
    std::cout << "  build systems must track BMI/IFC deps (CMake/MSVC/Clang)\n";
    std::cout << "  ABI: module interface change still breaks dependents\n";
    std::cout << "  this example stays on includes for portable CI\n";
    std::cout << "modules_import_std_impact_on_compile_model_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/g/section02/modules_import_std_impact_on_compile_model_cpp23", run>;

}  // namespace
