# LearnCpp · 现代 C++23 学习占位脚手架

按 `C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\Cpp-Modern-完整学习路线图-C++23.md`
逐阶段铺好的"占位 cpp + 自动测试 + CI" 框架. 后续自己往每个占位 `.cpp` 里
填例子 + 断言, `ctest` 看绿即视为该小项打完.

`module` 主体已剔除; `import std` 作为例外保留在 `src/engineering_system/import_std_exception/`.

## 现状

- C++ 标准: **C++23** (`set(CMAKE_CXX_STANDARD 23)`, `/std:c++latest`).
- 编译器目标:
  - **Windows 本机**: VS2026 Dev Prompt 18.x + clang-cl 22.1.3 + MSVC STL.
  - **Linux CI**: clang/clang++ (apt clang-19+).
  - **macOS CI**: clang/clang++ (Homebrew llvm@19+ 或系统 Apple clang).
- 构建系统: **CMake 3.28+** + **Ninja** + **sccache** (无 FASTBuild). GoogleTest v1.17 via `FetchContent`.
- CI: GitHub Actions 三套 workflow (`windows-ci.yml`, `linux-ci.yml`, `macos-ci.yml`),
  全 clang 系列, 全用 `cmake --preset *-ci`.
- Pre-commit 钩子: `trailing-whitespace` / `end-of-file-fixer` / `mixed-line-ending` /
  `check-{json,toml,yaml}` / `codespell` / `clang-format` / `clang-tidy`.
- 占位规模: 23 阶段目录 + ~900 个 `*.cpp` 占位符, 每个 `GTEST_SKIP()` 兜底, 编译能过、ctest 标 SKIPPED.

## 关键路径

```
.
├── .clang-format / .clang-tidy / .clangd / .codespell-ignore
├── .editorconfig / .gitattributes / .gitignore
├── .pre-commit-config.yaml
├── .github/workflows/{linux,macos,windows}-ci.yml
├── CMakeLists.txt              # 根 build 脚本, GLOB_RECURSE src/**/*.cpp
├── CMakePresets.json           # base + {windows,linux,macos} × {debug,release,ci}
├── cmake/                      # Sccache / CompilerWarnings / Sanitizers / StaticAnalysis / FetchGoogleTest
├── scripts/
│   ├── dev-shell.cmd           # 进 vcvars64 x64 dev prompt
│   └── configure-and-build.cmd # 一键 vcvars64 -> configure -> build -> ctest
└── src/                        # 全部学习占位
    ├── syntax_and_translation_model/                       # 阶段 1
    ├── type_system_value_category_deduction/               # 阶段 2
    ├── functions_overloading_lambdas/                      # 阶段 3
    ├── classes_and_object_model_basics/                    # 阶段 4
    ├── copy_move_smart_pointers/                           # 阶段 5
    ├── templates_and_generics/                             # 阶段 6
    ├── compile_time_programming/                           # 阶段 7
    ├── control_flow_and_modern_syntax/                     # 阶段 8
    ├── exceptions_error_handling_ub/                       # 阶段 9
    ├── stl_deep_dive/                                      # 阶段 10
    ├── concurrency_and_threading/                          # 阶段 11
    ├── coroutines/                                         # 阶段 12  (Modules 阶段 13 整体跳过)
    ├── keywords_and_features_index/                        # 阶段 14
    ├── classic_idioms/                                     # 阶段 15
    ├── standard_library_systematized/                      # 第 3 部分
    ├── engineering_system/                                 # 第 4 部分 (含 import_std_exception/)
    ├── evolution_history/                                  # 第 5 部分
    ├── deep_dive_object_model/                             # 第 6 部分 · 支线 A
    ├── deep_dive_lifetime_and_ownership/                   # 第 6 部分 · 支线 B
    ├── deep_dive_memory_management/                        # 第 6 部分 · 支线 C
    ├── deep_dive_name_lookup_overload_resolution/          # 第 6 部分 · 支线 D
    ├── deep_dive_templates_compile_time_system/            # 第 6 部分 · 支线 E
    ├── deep_dive_memory_model_and_concurrency/             # 第 6 部分 · 支线 F
    └── deep_dive_abi_compile_link_ub_performance/          # 第 6 部分 · 支线 G
```

每个阶段目录:

- 有一份 `README.md` (说明该阶段目标 + 上手节奏, 不写到每个子目录),
- 一层"大项"子目录 (如 `value_categories/`, `ranges_core_cpp20/`),
- 子目录里 N 个 `*.cpp` 占位 (一个小项一个文件), 每份是独立 TU + 一条 `GTEST_SKIP` 占位 test.

## 前置

`scoop` 已装的工具直接复用:

```pwsh
scoop install cmake ninja sccache llvm
```

- LLVM/clang-cl 也可走 VS2026 (`VC\Tools\Llvm\x64\bin\clang-cl.exe`), 二选一.
- Windows 编译/链接需要 `rc.exe` + UCRT/SDK headers, 必须在 **VS2026 Developer Command Prompt** 或本仓 `scripts\dev-shell.cmd` 里跑.
- macOS / Linux 不需要 vcvars, 直接 `clang++` 即可.

## 本地一把梭 (Windows)

```pwsh
# 方式 1: 用脚手架自带 helper, 内部会 call vcvars64
scripts\configure-and-build.cmd windows-debug

# 方式 2: 自己开发开发者命令提示符 (一次性):
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
TSan 不在默认开, 写并发题时手动加 `-DLEARNCPP_ENABLE_TSAN=ON` (若你扩展 `Sanitizers.cmake`).

## 把一个占位填完

1. `cd src/<stage>/<topic>/`, 选一个 `.cpp`;
2. 顶部注释里有 doc 引用, 翻到 `C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\<那个 md>` 看路线图;
3. 配合 `https://en.cppreference.com/cpp/...` 第一手 + `godbolt.org` 看汇编 + `cppinsights.io` 看 lowering;
4. 写最小例子, 把 `GTEST_SKIP()` 换成 `EXPECT_*` / `ASSERT_*`;
5. 跑这一个文件: `ctest --preset windows-debug -R '<stage>__<topic>\.<item>'`;
6. UB / 边界类: 配 `LEARNCPP_ENABLE_ASAN=ON` (Windows) 或 `LEARNCPP_ENABLE_UBSAN=ON` (Linux/macOS) 看 sanitizer 报点.

## 加一个新占位

直接 `cp` 一个现成 `.cpp` 改名 / 改 `Item :` 注释 / 改 `TEST(<suite>, <item>)` 里的 `<item>` 即可.
CMake 顶层 `file(GLOB_RECURSE CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")` 在下次 `cmake --build` 时自动接上.

## 工具版本备忘 (本机当前实测 2026-06)

```
clang-cl --version    # clang version 22.1.3, target x86_64-pc-windows-msvc
clang-tidy --version  # LLVM 22.1.3
cmake --version       # 3.31+
ninja --version       # 1.12+
sccache --version     # 0.10+
```

CI 用 LLVM 19 系列, 本机用 22.1.3; 都能过路线图 0.3 列出的 C++23 库 feature-test (`__cpp_lib_*`)
矩阵 (mdspan / flat_map / generator / expected / print / ranges::to / stacktrace 等). 例外:
`std::start_lifetime_as` 在 clang-cl 22.1.3 当前缺宏 (cl 19.51 有), 占位仍保留, 实做时按路线图降级.

## 与路线图的关系

- 本仓只放 **占位 + 自动测试 + CI** 三件套, **不在仓内**重写路线图.
- 路线图文档保留在原位 (`C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\`), 仓内通过注释 / README 引用相对路径.

## License

见 `LICENSE`.
