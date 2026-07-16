# C++ 示例资料与编写策略

本项目以 C++23 为基准，但“标准规定”“编译器支持”和“标准库支持”是三件不同的事。每个 topic 都应先确认语义，再确认目标工具链能力，最后用本仓实际构建验证。

## 资料优先级

按以下顺序解决冲突；低优先级资料不能推翻高优先级资料。

1. **C++23 标准文本**
   - [ISO/IEC 14882:2024](https://www.iso.org/standard/83626.html) 是正式标准。
   - [N4950](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf) 是 C++23 最终公开工作草案；[N4951 编辑报告](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4951.html) 说明其与 C++23 DIS 的关系。
   - 示例注释优先记录稳定的 clause 名称，如 `[basic.life]`、`[expr.const]`，而不是复制规范原文。
2. **WG21 提案与缺陷报告**
   - 从 [WG21 papers 索引](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/) 找提案，并使用实际并入 C++23 的 revision。
   - 提案用于理解问题、设计理由和边界；最终语义仍以标准文本及适用的缺陷报告为准。
3. **特性探测规范**
   - [SD-FeatureTest](https://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations) 是 `__cpp_*`、`__cpp_lib_*` 与对应提案的首选索引。
   - 库特性通常包含 `<version>` 后检查具体宏值；只检查 `__cplusplus` 不足以证明某项能力可用。
4. **实现状态与实现文档**
   - 语言前端：[Clang C++ status](https://clang.llvm.org/cxx_status.html)、[GCC C++ status](https://gcc.gnu.org/projects/cxx-status.html)、[MSVC conformance](https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance)。
   - 标准库：[MSVC STL](https://github.com/microsoft/STL) 及其 [Changelog](https://github.com/microsoft/STL/wiki/Changelog)、[libc++ C++23 status](https://libcxx.llvm.org/Status/Cxx23.html)、[libstdc++ status](https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2023)。
   - Windows 的 `clang-cl + MSVC STL` 必须分轴判断：语言能力看 Clang，库能力看 MSVC STL，再以 feature-test macro 和真实编译结果裁决。
5. **工程准则与日常参考**
   - [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) 用于审查接口、资源管理、生命周期、错误处理和并发设计；它不是语言规范。
   - [cppreference](https://en.cppreference.com/cpp/) 是优秀的社区参考，适合快速定位 API、复杂度和相关条款，但不是一手标准文本。语义有疑问时必须回到标准、提案和实现文档。

## 语义重写与版权边界

- 从资料中提取事实、约束和设计理由，使用新的场景、类型名、数据和控制流自行编写示例。
- 不复制标准正文、提案、cppreference、Core Guidelines、编译器测试套件或博客中的长段文字、完整示例和独特注释。
- 公开可读不等于允许无条件复制。即使来源采用开源许可证，直接复制也可能要求保留版权、许可证或 NOTICE；本项目默认采用语义重写。
- 可在源码中留下简短可核验线索，例如 `// References: [basic.life], P2590R2`，不要粘贴来源原文。
- 本地路线文档只能作为 topic 范围和教学意图的输入；迁入 `run()` 的代码仍需重新整理、补齐依赖并独立验证。

## 每个 topic 的最低覆盖

在主题允许的范围内，每个 `.cpp` 应尽量同时体现以下五层；小主题可以用一个紧凑场景合并表达。

1. **正常路径**：最小、可运行、能观察结果的典型用法。
2. **边界路径**：空输入、极值、生命周期边缘、异常路径、迭代器失效或复杂度转折等与主题相关的边界。
3. **常见误用**：展示错误原因和修正方式。故意 ill-formed 的代码不得进入正常编译分支，应改用 `requires`、concept、type trait、`static_assert` 或注释中的最小反例。
4. **标准保证**：用 `ExampleChecks`、`static_assert`、类型检查或简短注释区分“标准保证”“未指定”“实现定义”和“未定义行为”。
5. **实现差异**：只有确有差异时才加入，并通过标准宏、feature-test macro 或平台宏隔离；不要把某个实现的当前行为写成标准保证。

示例应从入门观察逐步到专家边界，但不能用不可达的大段样板堆砌“覆盖率”。优先使用确定性输入、Release 也生效的 `ExampleChecks`、`static_assert` 和必要的清晰输出，使全量运行可重复。

## 可编译性与安全规则

- 每个 topic 必须在当前统一构建中独立编译，并能与其他 translation unit 共存；辅助实体置于匿名命名空间，避免 ODR 冲突。
- 所需标准头必须显式 `#include`，不得依赖传递包含。
- C++23 库设施先检查相应 `__cpp_lib_*` 的最低值；不可用时提供仍能编译的教学性降级路径，并明确输出“当前工具链不支持”。
- 没有标准 feature-test macro 的设施可结合 `__has_include` 和受约束的编译探测，但不得仅按编译器版本猜测。
- UB 示例默认不得执行。优先展示安全对照；必须保留的危险操作只能放在不参与编译的短反例或由明确的专用运行条件隔离，且不能影响 Debug 全量遍历。
- 并发示例必须有确定的终止条件；避免数据竞争、无限等待、依赖时序的断言和不可控的大量输出。
- 性能示例只解释机制，不用一次运行结果宣称普遍性能结论。

## 无 `import` 原则

本仓 topic 使用传统标准头，不写 `import std;`、`import std.compat;`、模块声明或 header-unit import。原因是当前所有 topic 被聚合为一个普通可执行文件，而模块还要求编译器、标准库模块产物和构建系统协同配置。

即使 topic 名称讨论 `import std`，也只演示支持探测、headers 对照和生态边界；真正的模块编译实验应是独立工程，不进入本仓统一构建。不得为路线文档中存在但仓内没有的 Modules/import 条目新建 topic。

## 验收顺序

1. 对照本地路线文档确认 topic 边界。
2. 用 N4950 clause 和已采纳提案核验语义。
3. 查 SD-FeatureTest 以及对应编译器、标准库状态。
4. 自行重写并适配 `run(int, char**)`。
5. 单 topic 运行，再做全仓编译和 Debug 全量运行。

支持表可能滞后，最终事实始终是：特性宏满足要求，并且当前仓库在目标工具链上真实编译、链接和运行通过。
