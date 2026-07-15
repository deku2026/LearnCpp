# C++ Topic 手工填充计划（C++ Primer 级 · 并行独立编排）

**Branch / worktree:** `feat/fill-cpp-topic-examples` @
`C:\MyFile\ArcForges\LearnCpp\.worktrees\fill-cpp-examples`

**Goal:** 874 个占位 `.cpp` 全部按知识点独立写成可运行教学示例；编译通过；commit + push；**不开 PR**。

---

## 铁律

1. **禁止**脚本/模板批量灌正文（Python 关键字匹配等一律否决）。
2. **一题一案**：每个文件只服务自己的 item；场景、类型名、断言、讲解路径必须独立。
3. **先文档、后检索、再落笔**：
   - 主源：`C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\<对应.md>`
   - 标准：cppreference / ISO 要点
   - 扩展：Boost.Asio/Coroutine2/Beast、libunifex、folly、LLVM/clang 文档、提案（P 编号）——**按主题需要**深入，不是每个文件都堆 Boost
4. **谱系完整**：入门最小正确用法 → 常见变形与坑 → 专家边界（标准细则、C++11–23 演变、UB 安全对照、工程取舍）。
5. **可运行**：`assert` + 必要输出；不故意触发 UB；特性缺失用 feature-test 宏降级并打印说明。
6. **骨架不动**：`namespace { int run(int,char**); learn::topic<"id", run>; }` + 已有 topic id。

---

## 文档 ↔ 目录映射

| 目录 | 文档 | 文件数 |
|------|------|--------|
| part2_stage01_syntax_and_translation_model | 第2部分-阶段1-语法基础与翻译模型.md | 19 |
| part2_stage02_type_system_value_category_deduction | 第2部分-阶段2-类型系统-值类别与类型推导.md | 53 |
| part2_stage03_functions_overloading_lambdas | 第2部分-阶段3-函数-重载-lambda.md | 35 |
| part2_stage04_classes_and_object_model_basics | 第2部分-阶段4-类与对象模型基础.md | 47 |
| part2_stage05_copy_move_smart_pointers | 第2部分-阶段5-拷贝与移动语义-智能指针.md | 32 |
| part2_stage06_templates_and_generics | 第2部分-阶段6-模板与泛型编程.md | 40 |
| part2_stage07_compile_time_programming | 第2部分-阶段7-编译期编程.md | 21 |
| part2_stage08_control_flow_and_modern_syntax | 第2部分-阶段8-控制流与现代语法增强.md | 20 |
| part2_stage09_exceptions_error_handling_ub | 第2部分-阶段9-异常-错误处理与UB纪律.md | 45 |
| part2_stage10_stl_deep_dive | 第2部分-阶段10-STL深潜.md | 132 |
| part2_stage11_concurrency_and_threading | 第2部分-阶段11-并发与多线程.md | 35 |
| part2_stage12_coroutines | 第2部分-阶段12-协程.md | 32 |
| part2_stage14_keywords_and_features_index | 第2部分-阶段14-特性与关键字全表.md | 18 |
| part2_stage15_classic_idioms | 第2部分-阶段15-C++经典惯用法.md | 22 |
| part3_standard_library_systematized | 第3部分-标准库系统化.md | 86 |
| part4_engineering_system | 第4部分-工程系统.md | 52 |
| part6_branch_a … g | 支线 A–G 对应文档 / 路线图 | 16–41 |
| （后补）stage13 / part7 / part8 | 阶段13 / 第7 / 第8 | 文档有、目录无 |

---

## 单文件工作流（每个 agent / 每个 item）

```text
1. 打开占位 .cpp → 读 Topic id / Item / Section
2. 在对应 CppStudy 文档中定位同名概念/步骤/代码块
3. 检索：cppreference 该页 + 相关关键字/语法/特性/提案
   若主题涉及协程/异步/泛型协议 → 查 Boost.Asio/Coroutine2、标准 generator 等
4. 列出本文件必须覆盖的要点清单（自己列，不抄别的文件）
5. 手写独立示例（§入门 / §进阶 / §专家），断言验证
6. 自检：删掉兄弟文件后，单看本文件能否讲清该点
```

---

## 并行波次

| Wave | 并行单元 | 说明 |
|------|----------|------|
| W1 | stage01, 07, 08, 14, 15, branch_a | 体量小，先立质量样板 |
| W2 | stage02×2(section 切分), 03, 04, 05 | 语言核心 |
| W3 | stage06, 09, 11, 12 | 模板/异常/并发/协程 |
| W4 | stage10×4(按 section 切), part3×2 | STL 大户 |
| W5 | part4, branch_b–g | 工程 + 深水 |
| W6 | 补 stage13/part7/part8 骨架+示例 | 文档缺口 |
| W7 | vcvars64 全量编译修复 → commit → push | 收口 |

---

## 编译

```bat
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
cd /d C:\MyFile\ArcForges\LearnCpp\.worktrees\fill-cpp-examples
cmake --preset windows-debug
cmake --build --preset windows-debug -j
```

---

## 完成标准

- [ ] `src/cpp` 下 topic 无 `TODO: read cppreference`
- [ ] 每个文件有独立、可运行、分层次示例
- [ ] `learn_cpp` 全量链接通过
- [ ] 分支已 push，无 PR
