# LearnCpp · 现代 C++23 学习占位脚手架

按 `C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/Cpp-Modern-完整学习路线图-C++23.md`
逐阶段铺好的 "占位 cpp + 自动测试 + CI" 框架. 后续往每个占位 `.cpp` 里
填例子 + 断言, `ctest` 看绿即视为该小项打完.

`module` 主体已剔除; `import std` 作为例外保留在
`src/part4_engineering_system/section07_import_std_exception/`.

## 现状

- C++ 标准: **C++23** (`set(CMAKE_CXX_STANDARD 23)`, `/std:c++latest`).
- 编译器目标:
  - **Windows 本机**: VS2026 Dev Prompt 18.x + clang-cl 22.1.3 + MSVC STL.
  - **Linux CI**: clang/clang++ (apt clang-19+).
  - **macOS CI**: clang/clang++ (Homebrew llvm@19+ 或系统 Apple clang).
- 构建系统: **CMake 3.28+** + **Ninja** + **sccache** (无 FASTBuild). GoogleTest v1.17 via `FetchContent`.
- CI: GitHub Actions 三套 workflow (`windows-ci.yml`, `linux-ci.yml`, `macos-ci.yml`),
  全 clang 系列, 全用 `cmake --preset *-ci`.
- 钩子: `pre-commit` (`trailing-whitespace` / `end-of-file-fixer` / `mixed-line-ending`
  / `check-{json,toml,yaml}` / `codespell` / `clang-format`). clang-tidy 通过 CMake 选项启动,
  不放进 pre-commit (900 placeholders 跑一遍太久).
- 占位规模: 23 stage 目录 / 150 section 子目录 / ~880 个 `*.cpp` 占位 (各自一个 `GTEST_SKIP()` 兜底).

## 关键路径

```
.
├── .clang-format / .clang-tidy / .clangd / .codespell-ignore
├── .editorconfig / .gitattributes / .gitignore
├── .pre-commit-config.yaml
├── .github/workflows/{linux,macos,windows}-ci.yml
├── CMakeLists.txt              # 根 build 脚本, GLOB_RECURSE src/**/*.cpp
├── CMakePresets.json           # base + {windows,linux,macos} x {debug,release,ci}
├── cmake/                      # Sccache / CompilerWarnings / Sanitizers / StaticAnalysis / FetchGoogleTest
├── scripts/
│   ├── dev-shell.cmd           # 进 vcvars64 x64 dev prompt
│   └── configure-and-build.cmd # 一键 vcvars64 -> configure -> build -> ctest
└── src/                        # 全部学习占位 (一一对应路线图 23 份文档)
    ├── part2_stage01_syntax_and_translation_model/                # 第2部分-阶段1
    ├── part2_stage02_type_system_value_category_deduction/        # 第2部分-阶段2
    ├── part2_stage03_functions_overloading_lambdas/               # 第2部分-阶段3
    ├── part2_stage04_classes_and_object_model_basics/             # 第2部分-阶段4
    ├── part2_stage05_copy_move_smart_pointers/                    # 第2部分-阶段5
    ├── part2_stage06_templates_and_generics/                      # 第2部分-阶段6
    ├── part2_stage07_compile_time_programming/                    # 第2部分-阶段7
    ├── part2_stage08_control_flow_and_modern_syntax/              # 第2部分-阶段8
    ├── part2_stage09_exceptions_error_handling_ub/                # 第2部分-阶段9
    ├── part2_stage10_stl_deep_dive/                               # 第2部分-阶段10
    ├── part2_stage11_concurrency_and_threading/                   # 第2部分-阶段11
    ├── part2_stage12_coroutines/                                  # 第2部分-阶段12 (Modules 阶段 13 整体跳过)
    ├── part2_stage14_keywords_and_features_index/                 # 第2部分-阶段14
    ├── part2_stage15_classic_idioms/                              # 第2部分-阶段15
    ├── part3_standard_library_systematized/                       # 第3部分
    ├── part4_engineering_system/                                  # 第4部分 (含 section07_import_std_exception)
    ├── part6_branch_a_object_model/                               # 第6部分 / 支线A
    ├── part6_branch_b_lifetime_and_ownership/                     # 第6部分 / 支线B
    ├── part6_branch_c_memory_management/                          # 第6部分 / 支线C
    ├── part6_branch_d_name_lookup_overload_resolution/            # 第6部分 / 支线D
    ├── part6_branch_e_templates_compile_time_system/              # 第6部分 / 支线E
    ├── part6_branch_f_memory_model_and_concurrency/               # 第6部分 / 支线F
    └── part6_branch_g_abi_compile_link_ub_performance/            # 第6部分 / 支线G
```

每个 stage 目录:

- 一份 `README.md` (说明该 stage 目标 + 学习节奏);
- 一层 `sectionNN_xxx/` 子目录, 对应路线图里的"大项";
- 子目录内 N 个 `*.cpp` 占位 (一个小项一个文件), 每份是独立 TU + 一条 `GTEST_SKIP` 占位 test.

测试命名: `TEST(<stage_short>_<section_short>, <item>)`, 如
`TEST(part2_stage01_section01, main_and_program_structure)`. `ctest -R 'part2_stage10_'`
就能只跑 STL 阶段, `ctest -R 'part2_stage10_section08_'` 只跑 Ranges-C++23 节.

## 前置

scoop 已装的工具直接复用:

```pwsh
scoop install cmake ninja sccache llvm
```

- LLVM/clang-cl 也可走 VS2026 (`VC/Tools/Llvm/x64/bin/clang-cl.exe`), 二选一.
- Windows 编译/链接需要 `rc.exe` + UCRT/SDK headers, 必须在 **VS2026 Developer Command Prompt** 或本仓 `scripts/dev-shell.cmd` 里跑.
- macOS / Linux 不需要 vcvars, 直接 `clang++` 即可.

## 本地一把梭 (Windows)

```pwsh
# 方式 1: 用脚手架自带 helper, 内部会 call vcvars64
scripts\configure-and-build.cmd windows-debug

# 方式 2: 自己开发开发者命令提示符:
scripts\dev-shell.cmd            # %comspec% /k vcvars64.bat
# 进去之后:
cmake --preset windows-debug
cmake --build --preset windows-debug
ctest --preset windows-debug --output-on-failure

# 想加 ASan:
cmake --preset windows-debug -DLEARNCPP_ENABLE_ASAN=ON
```

## 本地一把梭 (Linux / macOS)

```bash
cmake --preset linux-debug      # 或 macos-debug
cmake --build --preset linux-debug
ctest --preset linux-debug --output-on-failure
```

Linux 的 CI 通道额外打开 `LEARNCPP_ENABLE_UBSAN=ON` (见 `linux-ci` preset).

## Pre-commit 钩子

```bash
pip install --user pre-commit          # 或 pipx install pre-commit / scoop install pre-commit
pre-commit install                     # 装到 .git/hooks
pre-commit run --all-files             # 全量跑一次
```

启用的钩子见 `.pre-commit-config.yaml`:
trailing-whitespace / end-of-file-fixer / mixed-line-ending /
check-{json,toml,yaml} / codespell / clang-format. clang-tidy 不放进钩子
(900 文件全跑要很久), 通过 `cmake -DLEARNCPP_ENABLE_CLANG_TIDY=ON` 在编译时启动.

## 把一个占位填完

1. `cd src/<stage>/<section>/`, 选一个 `.cpp`;
2. 顶部注释里有 doc 引用, 翻到 `C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/<那个 md>` 看路线图;
3. 配合 `https://en.cppreference.com/cpp/...` + `godbolt.org` 看汇编 + `cppinsights.io` 看 lowering;
4. 写最小例子, 把 `GTEST_SKIP()` 换成 `EXPECT_*` / `ASSERT_*`;
5. 跑这一个文件: `ctest --preset windows-debug -R '<stage_short>_<section_short>\.<item>'`;
6. UB / 边界类: 配 `LEARNCPP_ENABLE_ASAN=ON` (Windows) 或 `LEARNCPP_ENABLE_UBSAN=ON` (Linux/macOS).

## 加一个新占位

直接 `cp` 一个现成 `.cpp` 改名 / 改 `Item    :` 注释 / 改 `TEST(<suite>, <item>)` 里的 `<item>` 即可.
CMake 顶层 `file(GLOB_RECURSE CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")` 在下次 `cmake --build` 时自动接上.

## 工具版本备忘 (本机当前实测 2026-06)

```
clang-cl --version    # clang version 22.1.3, target x86_64-pc-windows-msvc
clang-tidy --version  # LLVM 22.1.3
cmake --version       # 3.31+
ninja --version       # 1.12+
sccache --version     # 0.10+
```

CI 用 LLVM 19/20 系列, 本机用 22.1.3; 都能过路线图 0.3 列出的 C++23 库 feature-test
(`__cpp_lib_*`) 矩阵 (mdspan / flat_map / generator / expected / print / ranges::to / stacktrace 等).

## 与路线图的关系

- 本仓只放 **占位 + 自动测试 + CI** 三件套, **不在仓内**重写路线图.
- 路线图文档保留在原位 (`C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/`), 仓内通过注释 / README 引用相对路径.

## License

见 `LICENSE`.
