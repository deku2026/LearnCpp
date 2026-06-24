# LearnCpp · 现代 C++23 学习占位脚手架

按 `C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/Cpp-Modern-完整学习路线图-C++23.md`
逐阶段铺好的 **874 个空 `run()` 占位**。后续往每个 `.cpp` 里直接写真实代码 (`std::cout` /
`assert` / 抛异常 / 玩 `argv`，怎么舒服怎么写)，**没有测试框架的盒子约束**。

`module` 主体已剔除；`import std` 作为例外保留在
`src/part4_engineering_system/section07_import_std_exception/`。

## 工作原理

整仓一个可执行 `learn_cpp`。每个占位 `.cpp` 长这样：

```cpp
#include "learn/topic_registry.hpp"

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    return 0;
}

LEARN_TOPIC("part2/stage01/section01/main_and_program_structure", run);

}  // namespace
```

`LEARN_TOPIC` 是个静态注册宏，在静态初始化阶段把 `(id, run)` 塞进全局 `std::map`。
`main()` 把命令行参数转给 registry 调度，**等同于直接进入那个 topic 的 `int main(int, char**)`**。

```pwsh
# Debug build 无参 = 遍历全部 874 个 topic (F5 from IDE 用这条)
build\windows-debug\bin\learn_cpp.exe

# Release/ci build 无参 = 列出全部 topic
build\windows-ci\bin\learn_cpp.exe

# 任意 build 跑某个 topic
build\windows-debug\bin\learn_cpp.exe part2/stage01/section01/main_and_program_structure

# 透传额外参数
build\windows-debug\bin\learn_cpp.exe part2/stage10/section08/ranges_to extra args
```

「无参」行为按 `NDEBUG` 宏分支:

- **Debug build** (`windows-debug`, `CMAKE_BUILD_TYPE=Debug`, 不定义 `NDEBUG`):
  无参时 registry 遍历全 map, 顺序调用每个 `run()`. IDE 里 F5 不用配 args, 任何 topic 里下的断点都会命中.
- **Release / RelWithDebInfo** (`*-release` / `*-ci`, 定义 `NDEBUG`):
  无参时只列出已注册 topic — 手动 pick.

调试时: IDE 的 launch profile 用 Debug build, 直接 F5; 或显式把 args 设成 topic id, F5 命中那一个 `run()` 里的断点.

## 关键路径

```
.
├── .clang-format / .clang-tidy / .clangd / .codespell-ignore
├── .editorconfig / .gitattributes / .gitignore
├── .pre-commit-config.yaml
├── .github/workflows/{linux,macos,windows}-ci.yml  # configure -> build -> smoke run learn_cpp
├── CMakeLists.txt              # 根 build 脚本, GLOB_RECURSE src/**/*.cpp
├── CMakePresets.json           # base + {windows,linux,macos} x {debug,release,ci}
├── cmake/                      # Sccache / CompilerWarnings / Sanitizers / StaticAnalysis
├── include/
│   └── learn/topic_registry.hpp  # LEARN_TOPIC 宏 + run_topic / list_topics 声明
├── scripts/
│   ├── dev-shell.cmd           # 进 vcvars64 x64 dev prompt
│   └── configure-and-build.cmd # 一键 vcvars64 -> configure -> build -> 列 topic
└── src/
    ├── main.cpp                # 整仓唯一 main(), 把 argv 转给 registry
    ├── learn/topic_registry.cpp   # registry 实现
    ├── part2_stage01_syntax_and_translation_model/               # 第2部分-阶段1
    ├── part2_stage02_type_system_value_category_deduction/       # 第2部分-阶段2
    ├── part2_stage03_functions_overloading_lambdas/              # 第2部分-阶段3
    ├── part2_stage04_classes_and_object_model_basics/            # 第2部分-阶段4
    ├── part2_stage05_copy_move_smart_pointers/                   # 第2部分-阶段5
    ├── part2_stage06_templates_and_generics/                     # 第2部分-阶段6
    ├── part2_stage07_compile_time_programming/                   # 第2部分-阶段7
    ├── part2_stage08_control_flow_and_modern_syntax/             # 第2部分-阶段8
    ├── part2_stage09_exceptions_error_handling_ub/               # 第2部分-阶段9
    ├── part2_stage10_stl_deep_dive/                              # 第2部分-阶段10
    ├── part2_stage11_concurrency_and_threading/                  # 第2部分-阶段11
    ├── part2_stage12_coroutines/                                 # 第2部分-阶段12 (Modules 阶段 13 整体跳过)
    ├── part2_stage14_keywords_and_features_index/                # 第2部分-阶段14
    ├── part2_stage15_classic_idioms/                             # 第2部分-阶段15
    ├── part3_standard_library_systematized/                      # 第3部分
    ├── part4_engineering_system/                                 # 第4部分 (含 section07_import_std_exception)
    ├── part6_branch_a_object_model/                              # 第6部分 / 支线A
    ├── part6_branch_b_lifetime_and_ownership/                    # 第6部分 / 支线B
    ├── part6_branch_c_memory_management/                         # 第6部分 / 支线C
    ├── part6_branch_d_name_lookup_overload_resolution/           # 第6部分 / 支线D
    ├── part6_branch_e_templates_compile_time_system/             # 第6部分 / 支线E (主索引内嵌, 没独立 md)
    ├── part6_branch_f_memory_model_and_concurrency/              # 第6部分 / 支线F
    └── part6_branch_g_abi_compile_link_ub_performance/           # 第6部分 / 支线G
```

每个 stage 目录:

- 一份 `README.md` (该 stage 目标 + 学习节奏);
- 一层 `sectionNN_xxx/` 子目录, 对应路线图里的「大项」;
- 子目录内 N 个 `*.cpp` 占位, 每份 = 独立 TU + 一条 `LEARN_TOPIC(...)` 注册。

## 现状

- C++ 标准: **C++23** (`set(CMAKE_CXX_STANDARD 23)`, `/std:c++latest`).
- 编译器目标:
  - **Windows 本机**: VS2026 Dev Prompt 18.x + clang-cl 22.1.3 + MSVC STL.
  - **Linux**: clang/clang++ 22 (CI 走 apt.llvm.org 装 clang-22, 锁主版本).
  - **macOS**: clang/clang++ 22 (CI 走 Homebrew llvm@22, 锁主版本).
  - **Windows CI**: 用 `windows-2022` runner 自带 LLVM (~20.x, 不锁版本; 本地是 VS2026 自带 clang-cl 22.1.3, 跟 CI 不一致, 接受).
- 构建系统: **CMake 3.28+** + **Ninja** + **sccache**.
- CI: GitHub Actions 三套 workflow (`windows-ci.yml` / `linux-ci.yml` / `macos-ci.yml`),
  每条都 configure + build + smoke-run `learn_cpp` (列出 topic 数). **没有跑 gtest / ctest** —
  代码主体是赤裸 `int run(int, char**)`, CI 守的是"874 TU 还能在三平台编出来 + registry 还能跑"。
- 钩子: `pre-commit` (`trailing-whitespace` / `end-of-file-fixer` / `mixed-line-ending`
  / `check-{json,toml,yaml}` / `codespell` / `clang-format`). clang-tidy 通过 CMake 选项开,
  不放进 pre-commit (874 文件全跑太久)。
- 占位规模: **23 stage / 150 section / 874 个 `.cpp`**, 每个一条 `LEARN_TOPIC` 注册。

## 前置

scoop 已装的工具直接复用:

```pwsh
scoop install cmake ninja sccache llvm
```

- LLVM/clang-cl 也可走 VS2026 (`VC/Tools/Llvm/x64/bin/clang-cl.exe`), 二选一。
- Windows 编译/链接需要 `rc.exe` + UCRT/SDK headers, 必须在 **VS2026 Developer Command Prompt** 或本仓 `scripts/dev-shell.cmd` 里跑。
- macOS / Linux 不需要 vcvars, 直接 `clang++` 即可。

## 本地一把梭 (Windows)

```pwsh
# 方式 1: 用脚手架自带 helper, 内部 call vcvars64
scripts\configure-and-build.cmd windows-debug                        # 编完列 topic
scripts\configure-and-build.cmd windows-debug part2/stage01/section01/main_and_program_structure   # 编完直接跑

# 方式 2: 自己开发者命令提示符
scripts\dev-shell.cmd
cmake --preset windows-debug
cmake --build --preset windows-debug
build\windows-debug\bin\learn_cpp.exe                                # 列 topic
build\windows-debug\bin\learn_cpp.exe part2/stage01/section01/main_and_program_structure

# 加 ASan
cmake --preset windows-debug -DLEARNCPP_ENABLE_ASAN=ON
```

## 本地一把梭 (Linux / macOS)

```bash
cmake --preset linux-debug          # 或 macos-debug
cmake --build --preset linux-debug
build/linux-debug/bin/learn_cpp     # 列 topic
build/linux-debug/bin/learn_cpp part2/stage01/section01/main_and_program_structure
```

## Pre-commit 钩子

```bash
pip install --user pre-commit          # 或 pipx install pre-commit / scoop install pre-commit
pre-commit install                     # 装到 .git/hooks
pre-commit run --all-files             # 全量跑一次
```

启用的钩子见 `.pre-commit-config.yaml`。clang-tidy 不放进钩子 (874 文件全跑要很久),
通过 `cmake -DLEARNCPP_ENABLE_CLANG_TIDY=ON` 在编译时启动。

## 填一个占位

1. 选一个 `.cpp` (例: `src/part2_stage02_.../section05_value_categories/lvalue_xvalue_prvalue.cpp`);
2. 顶部注释里有 doc slug 和 topic id, 翻 `C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/<那个 md>` 看路线图本节;
3. 配合 `https://en.cppreference.com/cpp/...` + `godbolt.org` 看汇编 + `cppinsights.io` 看 lowering;
4. 在 `run(int argc, char** argv)` 里直接写代码 — `std::cout` / `std::print` / `assert` / 抛异常 / 用 argv 都可;
5. 跑: `build\windows-debug\bin\learn_cpp.exe <topic id>`;
6. UB / 边界类: 配 `LEARNCPP_ENABLE_ASAN=ON` (Windows) 或 `LEARNCPP_ENABLE_UBSAN=ON` (Linux/macOS) 重新 build 再跑。

## 加一个新占位

直接 `cp` 一个现成 `.cpp` 改名, 改顶部 4 个注释字段 (Doc / Stage / Section / Item / Topic id),
改 `LEARN_TOPIC("<新 id>", run)`。CMake 顶层 `file(GLOB_RECURSE CONFIGURE_DEPENDS "src/*.cpp")`
在下次 `cmake --build` 时自动接上。

## 工具版本备忘 (本机当前实测 2026-06)

```
clang-cl --version    # clang version 22.1.3, target x86_64-pc-windows-msvc
clang-tidy --version  # LLVM 22.1.3
cmake --version       # 3.31+
ninja --version       # 1.12+
sccache --version     # 0.10+
```

## 与路线图的关系

- 本仓只放 **占位 + registry + 本地工具链**, **不在仓内**重写路线图。
- 路线图文档保留在原位 (`C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/`), 仓内通过注释 / README 引用相对路径。

## License

见 `LICENSE`。
