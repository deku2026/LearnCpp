// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.6 模块与 import std 构建支持
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : cmake_modules_and_import_std_cpp23
// Topic id : part4/section01/cmake_modules_and_import_std_cpp23
//
// 要点: CMake 3.28+ FILE_SET CXX_MODULES; import std 需 3.30+ 实验开关
//       + Ninja。工程默认仍 headers; 本仓库 CMAKE 3.28 基线。
// 探测: 语言/库 feature-test 宏; 不真正 import std(可移植兜底)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <version>

namespace {

struct CMakeModuleSupport {
    int cmake_major;
    int cmake_minor;
    bool has_cxx_modules_fileset;    // 3.28+
    bool has_import_std_experiment;  // 3.30+ 实验
};

CMakeModuleSupport support_for(int major, int minor) {
    CMakeModuleSupport s{major, minor, false, false};
    if (major > 3 || (major == 3 && minor >= 28)) {
        s.has_cxx_modules_fileset = true;
    }
    if (major > 3 || (major == 3 && minor >= 30)) {
        s.has_import_std_experiment = true;
    }
    return s;
}

// 本仓库 CMakePresets cmakeMinimumRequired = 3.28.0
bool repo_baseline_ok(int major, int minor, int patch) {
    if (major != 3) {
        return major > 3;
    }
    if (minor != 28) {
        return minor > 28;
    }
    return patch >= 0;
}

void probe_language_macros(std::ostream& os) {
#ifdef __cpp_modules
    os << "  __cpp_modules=" << __cpp_modules << '\n';
#else
    os << "  __cpp_modules=(undefined)\n";
#endif
#ifdef __cpp_lib_modules
    os << "  __cpp_lib_modules=" << __cpp_lib_modules << '\n';
#else
    os << "  __cpp_lib_modules=(undefined)\n";
#endif
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== cmake_modules_and_import_std_cpp23 ===\n";

    // --- 入门: 版本门槛 ---
    auto s28 = support_for(3, 28);
    assert(s28.has_cxx_modules_fileset);
    assert(!s28.has_import_std_experiment);

    auto s30 = support_for(3, 30);
    assert(s30.has_cxx_modules_fileset && s30.has_import_std_experiment);

    auto s27 = support_for(3, 27);
    assert(!s27.has_cxx_modules_fileset);

    // 本仓库 cmake_minimum_required 3.28
    auto repo = support_for(3, 28);
    assert(repo.has_cxx_modules_fileset);
    assert(repo_baseline_ok(3, 28, 0));
    std::cout << "  repo baseline 3.28: modules FILE_SET=" << std::boolalpha << repo.has_cxx_modules_fileset
              << " import_std_exp=" << repo.has_import_std_experiment << '\n';

    // --- 进阶: 实验开关名 + 生成器 ---
    const std::string exp_var = "CMAKE_EXPERIMENTAL_CXX_IMPORT_STD";
    assert(exp_var.find("IMPORT_STD") != std::string::npos);
    const std::string generator_need = "Ninja";  // import std 主流路径
    assert(generator_need == "Ninja");
    // 文档纪律: 工程默认仍用 headers
    const bool default_use_headers = true;
    assert(default_use_headers);

    // --- 专家: 特性探测, 不假设「版本够了」 ---
    probe_language_macros(std::cout);

    // 可移植路径: 标准头(本文件所用) 始终可用
    assert(std::string_view{__FILE__}.find("cmake_modules") != std::string_view::npos || true);
    const int portable_surface = [] {
        // 等同 import std 后仍会用的 API
        return static_cast<int>(std::string{"headers"}.size());
    }();
    assert(portable_surface == 7);
    std::cout << "  portable path: #include <version> / headers (default)\n";
    std::cout << "  experimental: FILE_SET TYPE CXX_MODULES + import std\n";

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    std::cout << "  __cpp_lib_print available\n";
    assert(__cpp_lib_print >= 202207L);
#else
    std::cout << "  __cpp_lib_print missing or old — use iostream fallback\n";
#endif

    std::cout << "cmake_modules_and_import_std_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/cmake_modules_and_import_std_cpp23", run>;

}  // namespace
