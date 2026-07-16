# LearnCpp · 874 个现代 C++23 可运行示例

本仓把 `C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/` 中的现代 C++ 路线整理为一套可编译、可检索、可逐项运行的示例库：23 个阶段、150 个 section、874 个 topic，覆盖语法与类型系统、标准库、并发、协程、工程系统，以及对象模型、生命周期、模板、内存模型和 ABI/UB 等深水主题。

每个 topic 都是独立 `.cpp` 翻译单元，并保持与路线文档一一对应。示例采用传统标准头；仓库不使用模块声明、header-unit 或 `import std`。名称涉及模块生态的既有 topic 只讲能力边界与编译模型，不写实验性模块语法。

## 设计目标

- **真实可运行**：正常路径由 `learn::ExampleChecks` 验证，在 Debug 和 Release 中都不会因 `NDEBUG` 消失。
- **从基础到边界**：在现有 topic 范围内覆盖典型用法、边界条件、常见误用、标准保证及必要的实现差异。
- **安全全量遍历**：UB、悬空、越界、竞态、错误模板等只保留为停用反例或安全模型，正常执行路径不触发危险行为。
- **精确能力探测**：C++23 库设施使用对应 `__cpp_lib_*`/语言特性宏；当前工具链不支持时明确跳过，而不是按编译器版本猜测。
- **跨平台共存**：辅助实体置于匿名命名空间，头文件显式包含，874 个 topic 能同时链接进一个程序。

资料核验顺序、版权边界与示例验收标准见 [docs/reference-policy.md](docs/reference-policy.md)，topic 到路线文档的完整映射见 [docs/topic-document-map.md](docs/topic-document-map.md)。

## 工作原理

整仓生成一个 `learn_cpp` 可执行文件。每个 topic 定义 `run(int, char**)` 并注册唯一 id：

```cpp
#include "learn/example_support.hpp"

namespace {

constexpr std::string_view kTopic = "part2/stage01/section01/main_and_program_structure";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, 20 + 22, 42);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<
    "part2/stage01/section01/main_and_program_structure", run>;

}  // namespace
```

`learn::topic<Id, Fn>` 在静态初始化阶段把 `(id, run)` 放入注册表。命令行可以运行一个 topic；Debug 无参数运行时会按 id 顺序遍历全部 874 个 topic。

```pwsh
# Debug：无参数遍历全部 topic
build\windows-debug\bin\learn_cpp.exe

# 运行一个 topic，并可继续透传参数
build\windows-debug\bin\learn_cpp.exe `
  part2/stage01/section01/main_and_program_structure extra args

# Release：无参数列出全部 topic
build\windows-release\bin\learn_cpp.exe
```

单个示例失败会记录 topic id；Debug 全量遍历会继续执行后续主题并最终返回失败状态。预期异常使用 `LEARN_EXPECT_THROWS`，不支持的库特性使用 `ExampleChecks::unavailable` 明确输出跳过原因。

## 目录

```text
.
├── CMakeLists.txt / CMakePresets.json
├── cmake/                         # warnings、sanitizers、静态分析、sccache
├── docs/
│   ├── reference-policy.md        # 资料与示例编写策略
│   └── topic-document-map.md      # 874 topic 与 23 份路线文档映射
├── include/learn/
│   ├── example_support.hpp        # Release-safe 检查与参数辅助
│   └── topic_registry.hpp         # topic id、注册与调度接口
├── scripts/
│   ├── configure-and-build.cmd
│   ├── dev-shell.cmd
│   ├── generate-vs-filters.ps1
│   ├── run-all-topics.ps1         # 子进程隔离、超时与危险模式排除
│   ├── run-strict-build-matrix.sh  # Clang/GCC × libstdc++/libc++ 全量严格构建
│   └── validate-topic-catalog.ps1 # 数量、元数据、映射、语法与占位检查
├── src/
│   ├── main.cpp / learn/topic_registry.cpp
│   ├── part2_stage01_...          # 阶段 1–12、14–15；Modules 阶段 13 不入仓
│   ├── part3_standard_library_systematized/
│   ├── part4_engineering_system/
│   └── part6_branch_a_...g_...    # 七条深水支线 A–G
└── vs/learn_cpp.vcxproj           # 不依赖 CMake 的 VS 工程
```

## Windows 构建

需要 CMake、Ninja 和 VS 18 C++ 工具链。仓库 helper 会先进入 `vcvars64` 环境：

```pwsh
scripts\configure-and-build.cmd windows-debug

# 或手动执行
scripts\dev-shell.cmd
cmake --preset windows-debug
cmake --build --preset windows-debug
build\windows-debug\bin\learn_cpp.exe
```

也可以直接构建 Visual Studio 工程：

```pwsh
msbuild LearnCpp.slnx /m /p:Configuration=Debug /p:Platform=x64
msbuild LearnCpp.slnx /m /p:Configuration=Release /p:Platform=x64
```

MSVC 构建显式启用 UTF-8；链接时保留自注册 topic，避免 Release 的未引用 COMDAT 消除把注册表裁空。

## Linux / macOS 构建

```bash
cmake --preset linux-debug        # macOS 使用 macos-debug
cmake --build --preset linux-debug
build/linux-debug/bin/learn_cpp
```

项目基准为 C++23，但单项库特性仍以 feature-test macro 和真实构建为准。

### Linux / macOS 严格可移植性矩阵

严格脚本会分别执行 Debug 与 RelWithDebInfo 的干净构建，启用
`-Werror`、UBSan，并把 Ninja 设为 `-k 0`，使一次运行能收集全部翻译单元的
错误。Linux 覆盖 Clang 22 + libstdc++、Clang 22 + libc++、GCC 14 +
libstdc++；macOS 覆盖 Homebrew Clang 22 + libc++。任何矩阵项失败都会使脚本
最终返回非零，但不会阻止其余项继续检查。

```bash
bash scripts/run-strict-build-matrix.sh
```

默认要求 Clang 22 与 GCC 14，以便和 CI 工具链一致。自定义安装路径可显式传入，
脚本仍会核对主版本，避免命令名与实际编译器不一致：

```bash
LEARNCPP_CLANGXX=/opt/llvm/bin/clang++ \
LEARNCPP_GXX=/opt/gcc/bin/g++ \
bash scripts/run-strict-build-matrix.sh
```

可用 `LEARNCPP_STRICT_BUILD_TYPES=RelWithDebInfo` 只运行优化构建；默认不使用编译
缓存，确保每次都重新检查全部翻译单元。

## 目录与运行验收

```pwsh
# 严格验证 876 个翻译单元、874 个 topic、23 个阶段、文档映射、
# 唯一 id、无占位/TODO/空 run、无 module/import 语法
pwsh scripts\validate-topic-catalog.ps1

# 每个安全 topic 在独立子进程中运行，并带超时
pwsh scripts\run-all-topics.ps1 -Executable `
  build\windows-release\bin\learn_cpp.exe
```

全库构建可按需启用：

```pwsh
cmake --preset windows-debug -DLEARNCPP_WERROR=ON
cmake --preset windows-asan
cmake --build --preset windows-asan
```

Windows 的 clang-cl ASan 预设使用带调试信息的 Release CRT；该工具链不接受 Debug CRT 与 ASan 的组合。Linux/macOS 可额外使用 `LEARNCPP_ENABLE_UBSAN=ON`。Sanitizer 是探测器而不是正确性证明；示例本身仍必须保持定义良好。

## 维护规则

1. 不新增路线与仓库中都不存在的 topic；若确需改目录，先同步映射和期望计数。
2. 修改 topic 后先单项编译/运行，再执行严格 catalog、全量构建和隔离运行。
3. C++23 设施按准确宏值门控；不可用分支仍需有教学意义且能编译。
4. 禁止在正常路径执行 UB、数据竞争、无限等待或依赖偶然调度的断言。
5. 添加或删除源文件/头文件后运行 `scripts/generate-vs-filters.ps1`，保持 VS 项目与 CMake 收集结果一致。

## License

见 `LICENSE`。
