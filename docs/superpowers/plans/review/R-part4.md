# R-part4 · 工程系统 交叉 Review

**Date:** 2026-07-15
**Doc:** `C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\第4部分-工程系统.md`
**Code:** `src/cpp/part4_engineering_system/` (54 `.cpp`，含本轮 +2)
**Repo 对照:** `CMakePresets.json` + `cmake/{Sanitizers,CompilerWarnings,StaticAnalysis,Sccache,CompileCommands}.cmake`

---

## 1. 文档概念清单 L（摘要）

| 文档锚点 | 关键概念 / 验收点 |
|---|---|
| 4.1 CMake | target-based；PUBLIC/PRIVATE/INTERFACE；完整多目标；**FetchContent**；install/export；Ninja/ccache/PCH/Unity；模块/import std；**CMakePresets** |
| 4.2 包管理 | vcpkg manifest；Conan profile；CPM；semver/lock |
| 4.3 质量门禁 | clang-format；clang-tidy；cppcheck；IWYU；警告/-Werror |
| 4.4 测试 | GTest/Catch2/doctest；CTest；单测 vs 集成；解耦 Qt；Fuzzing |
| 4.5 Sanitizer | ASan/UBSan/TSan/MSan/LSan；混用矩阵；stacktrace；valgrind/profiler/coverage |
| 4.6 CI/CD | GH Actions 矩阵；编译器/构建型/Sanitizer 通道；feature 宏探测；跨平台 |
| 4.1.6 / §7 | import std 例外与兼容 |

**工程纪律（本仓库必须可运行探测，不能空讲）:** 每个 topic 用 C++ 小模型 / feature 宏 / 同步正确路径模拟配置语义，并**点名**对应 `cmake/*.cmake` 或 preset 字段。

---

## 2. 覆盖矩阵（Doc → Cpp）

| 文档块 | 覆盖 | 主要 topic | 仓库实体对照 |
|---|---|---|---|
| 4.1.1–4.1.2 target / 可见性 / include-link | covered | `cmake_target_basics`, `public_private_interface`, `target_link_include_directories`, `generator_expressions` | `CMakeLists.txt` target API |
| 4.1.3 FetchContent | **was missing → 已补** | **`fetch_content_declare`（新）**；`cpm_lightweight` 对照 | 文档 Declare/MakeAvailable/GIT_TAG |
| 4.1.4 install/export | covered | `install_export_find_package` | Config.cmake 心智模型 |
| 4.1.5 Ninja/cache/PCH/Unity | covered | `ninja_backend`, `ccache_versus_sccache`, `precompiled_headers`, `unity_build` | `base.generator=Ninja`；`Sccache.cmake` |
| 4.1.6 模块 / import std | covered | `cmake_modules_and_import_std_cpp23` + section07 | CMake 3.28 基线；实验开关名 |
| 4.1.7 CMakePresets | **弱 → 已补** | **`cmake_presets_json`（新）**；`multi_config` | 镜像 windows/linux-debug/ci 字段 |
| 4.2 包管理 | covered | `vcpkg_manifest`, `conan_profile`, `cpm_lightweight`, `semver_and_lockfiles` | 锁版本纪律 |
| 4.3 质量门禁 | covered | section03 全部 6 文件 | `CompilerWarnings.cmake`；`StaticAnalysis.cmake`；`LEARNCPP_WERROR` |
| 4.4 测试 | covered | section04 全部 10 文件 | GTest/Catch2/CTest/Fuzz 模型 |
| 4.5 Sanitizer | covered | section05 全部 10 文件 | `Sanitizers.cmake` ASAN/UBSAN |
| 4.6 CI | covered | section06 全部 7 文件 | preset `windows-ci`/`linux-ci` |
| import std 例外 | covered | section07 全部 4 文件 | 默认可移植 headers |

---

## 3. 与 cmake/*.cmake / Presets 对齐检查

| 仓库开关 / 文件 | 文档点 | topic 探测 |
|---|---|---|
| `LEARNCPP_USE_SCCACHE` + `Sccache.cmake` | 4.1.5 编译缓存 | `ccache_versus_sccache`, `cmake_presets_json` |
| `LEARNCPP_WERROR` + `CompilerWarnings.cmake` | 4.3.4 /W4 /WX | `warning_treatment_and_werror`；ci presets |
| `LEARNCPP_ENABLE_ASAN/UBSAN` + `Sanitizers.cmake` | 4.5 / 4.6 lanes | `asan_overview`, `ubsan_overview`, `asan_ubsan_lane`, `tsan_lane` |
| `LEARNCPP_ENABLE_CLANG_TIDY` + `StaticAnalysis.cmake` | 4.3.2 | `clang_tidy_setup` |
| `CMAKE_EXPORT_COMPILE_COMMANDS` + `CompileCommands.cmake` | IDE/tidy | presets 缓存变量心智；`cmake_presets_json` |
| `CMakePresets.json` version 9 / min 3.28 | 4.1.7 | `cmake_presets_json`, `cmake_modules_*` |
| generator Ninja | 4.1.5 | `ninja_backend` |

**纪律:** Sanitizer topic **不故意触发**溢出/数据竞争（避免本机 ASan 杀进程）；用安全 API + 注释 anti-pattern + 混用矩阵断言。

---

## 4. 质量抽检

| 文件 | 结论 |
|---|---|
| `public_private_interface.cpp` | 传播模型清晰 — STRONG |
| `sanitizer_compatibility_matrix.cpp` | ASan+UBSan / TSan 单独 — STRONG |
| `vcpkg_manifest.cpp` / `googletest_basics.cpp` | 可运行锁/测试模型 — PASS |
| `cmake_modules_and_import_std_cpp23.cpp` | 原 `assert(true)` → 真实版本门槛断言 — PASS |
| `fetch_content_declare.cpp` | 新：锁 tag / 拒 master / 与包管理分工 — PASS |
| `cmake_presets_json.cpp` | 新：镜像本仓 preset + flag 名 — PASS |
| `feature_macro_probe_in_ci_cpp23.cpp` | 扩展 C++23 soft features 矩阵 — PASS |
| `import_std_basic_cpp23.cpp` | readiness 结构体 + 默认 headers — PASS |

---

## 5. 本轮已做修改

### 新增

| 文件 | 文档锚点 | 说明 |
|---|---|---|
| `section01_build_system_cmake/fetch_content_declare.cpp` | 4.1.3 | 无对应 item → 新增；模拟 Declare/MakeAvailable；强制 GIT_TAG |
| `section01_build_system_cmake/cmake_presets_json.cpp` | 4.1.7 | 对照本仓 windows/linux-debug/ci；binaryDir；LEARNCPP_* 开关名 |

### 增强

- `cmake_modules_and_import_std_cpp23.cpp` — 去掉空断言；3.27/3.28/3.30 门槛；portable surface
- `import_std_basic_cpp23.cpp` — ImportStdReadiness；与仓库 3.28 基线对照
- `lsan_overview.cpp` — shared 环说明；与 ASan 捆绑；repo flag 名
- `tsan_lane.cpp` — matrix_valid；Linux CI 纪律；Sanitizers.cmake 对照
- `feature_macro_probe_in_ci_cpp23.cpp` — 更多 `__cpp_lib_*`；hard baseline vs soft
- `compiler_matrix_gcc_clang_msvc.cpp` — CI 车道映射；clang-cl 识别断言

---

## 6. 编译

- smoke：`build/part34_review_smoke` 对本轮改动/新增 TU **全部 `/c` 通过**。
- 全量 preset 构建受并行 ninja recompaction 权限干扰；CMake 已识别新增 TU（全局 880 sources）。

---

## 7. 残留 / 可选

| 项 | 说明 |
|---|---|
| 真实 `import std` | 仍实验；section07 明确 headers 默认 — 正确 |
| TSan CMake 选项 | 仓库 `Sanitizers.cmake` 仅 ASAN/UBSAN；TSan 以 CI lane 文档+topic 模型覆盖 |
| 真实下载 FetchContent | 教学禁止网络依赖；用锁版本模型足够 |

**目标状态:** 文档 4.1–4.6 无 missing；工程 topic 均有可运行探测；无 FAIL。
