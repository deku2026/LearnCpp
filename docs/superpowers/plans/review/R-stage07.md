# R-stage07 · 编译期编程 — 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Wave:** R2
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段7-编译期编程.md`
**Cpp root:** `src/cpp/part2_stage07_compile_time_programming/`
**Topic count:** 21

---

## 1. 文档概念清单 L（权威）

| # | 文档锚点 | 概念 | 验收/重点 |
|---|----------|------|-----------|
| L1 | 步骤 1.1–1.4 | constexpr 变量/函数/字面值类型/`static_assert` | factorial + static_assert |
| L2 | 步骤 1.5 | C++11→20 放宽简史 | 循环/if/虚函数/动态分配 |
| L3 | 步骤 2.1 | P2448 非字面值签名 | 参数/返回可非 literal |
| L4 | 步骤 2.2 | 永不常量求值的 constexpr 仍合法 | 泛型保守标注 |
| L5 | 步骤 2.3 | static/thread_local/goto 放宽（P2647/P2242） | static 查表 |
| L6 | 步骤 3 | consteval 立即函数 | 必须编译期；vs constexpr |
| L7 | 步骤 3.3 | P2564 immediate-escalating | lambda/模板升级；显式 constexpr 不升级 |
| L8 | 步骤 4 | constinit + SIOF | 强制常量初始化、可变、不能当常量表达式 |
| L9 | 步骤 5.1–5.2 | `is_constant_evaluated` + 致命陷阱 | **禁止** `if constexpr (ice())` |
| L10 | 步骤 5.3–5.4 | `if consteval`；true 分支可调 consteval | 编译期精确/运行期快路径 |
| L11 | 步骤 6 | constexpr vector/string；transient；unique_ptr；查表/sort | 不能泄漏堆到运行期 |
| L12 | 步骤 7 | constexpr to/from_chars；cmath 子集 | 编译期字符串↔数值 |
| L13 | 步骤 8 | TMP → constexpr 迁移 | 数值计算普通代码化 |

---

## 2. Cpp 文件清单 F

| Section | Files |
|---------|-------|
| section01_constexpr_evolution (6) | `constexpr_variable`, `constexpr_function_cpp11`, `constexpr_relaxations_cpp14_17_20`, `constexpr_non_literal_signatures_cpp23`, `constexpr_function_never_constant_evaluated_cpp23`, `constexpr_allows_static_thread_local_cpp23` |
| section02_if_consteval_cpp23 (2) | `if_consteval_versus_is_constant_evaluated`, `if_consteval_calls_consteval_function` |
| section03_consteval_constinit (3) | `consteval_immediate_functions`, `consteval_propagation_immediate_escalating_cpp23`, `constinit_static_init_order` |
| section04_static_assert_and_is_constant_evaluated (2) | `static_assert`, `std_is_constant_evaluated_cpp20` |
| section05_compile_time_containers_and_algorithms (7) | `constexpr_std_vector_cpp20`, `constexpr_std_string_cpp20`, `constexpr_unique_ptr_cpp23`, `compile_time_lookup_table`, `compile_time_sort`, `constexpr_to_chars_from_chars_cpp23`, `constexpr_cmath_cpp23_subset` |
| section06_tmp_versus_constexpr (1) | `migrate_tmp_to_constexpr` |

---

## 3. 覆盖矩阵 L × F

| L | 主要 cpp | 评级 | 备注 |
|---|----------|------|------|
| L1 | `constexpr_variable`, `constexpr_function_cpp11`, `static_assert` | **STRONG** | 字面值类型 + 编译期断言 |
| L2 | `constexpr_relaxations_cpp14_17_20` | **STRONG** | 分标准放宽实证 |
| L3 | `constexpr_non_literal_signatures_cpp23` | **PASS** | P2448 动机讲清 |
| L4 | `constexpr_function_never_constant_evaluated_cpp23` | **PASS** | feature 门控；永不 CTCE 仍合法 |
| L5 | `constexpr_allows_static_thread_local_cpp23` | **PASS+** | static 查表路径 |
| L6 | `consteval_immediate_functions` | **STRONG** | 运行期调用失败注释 |
| L7 | `consteval_propagation_immediate_escalating_cpp23` | **PASS** | P2564；工具链差异注释 |
| L8 | `constinit_static_init_order` | **STRONG** | 单 TU 模拟 SIOF；可变 vs constexpr |
| L9 | `std_is_constant_evaluated_cpp20`, `if_consteval_versus_*` | **STRONG** | 陷阱：if constexpr(ice) 恒 true |
| L10 | `if_consteval_calls_consteval_function`, `if_consteval_versus_*` | **STRONG** | newton 链 + 为何 ice 不能调 consteval |
| L11 | section05 vector/string/unique_ptr/table/sort | **STRONG** | transient 专家段；筛法工作区 |
| L12 | `constexpr_to_chars_from_chars_cpp23`, `constexpr_cmath_cpp23_subset` | **PASS+** | 库支持差异用 feature/运行期兜底 |
| L13 | `migrate_tmp_to_constexpr` | **STRONG** | Fib 模板 vs constexpr 对照 |

**FAIL:** 0　**WEAK:** 0

---

## 4. 检索补细节（if consteval / constexpr 边界）

| 主题 | 标准/提案 | 写入位置 |
|------|-----------|----------|
| if consteval 立即函数上下文 | P1938 / [stmt.if] | `if_consteval_calls_consteval_function` |
| ice 非立即上下文 → 不能调 consteval | [expr.const] / P0595 | 同文件注释「bad_with_ice」 |
| `if constexpr (is_constant_evaluated())` 恒 true | 常见陷阱 | `if_consteval_versus_is_constant_evaluated` |
| P2448 放宽 | 签名 / never-CTCE | section01 两文件 |
| P2564 consteval 向上传播 | immediate-escalating | `consteval_propagation_*` |
| transient allocation | P0784 模型 | `constexpr_std_vector_cpp20` |
| P2273 constexpr unique_ptr | + stage05 镜像 | `constexpr_unique_ptr_cpp23` |
| P2291 to/from_chars constexpr | 整数路径 | `constexpr_to_chars_from_chars_cpp23` |
| SIOF + constinit | P1143 | `constinit_static_init_order` |

**教学要点固化：**
`if consteval { call_consteval(x); }` 合法，因为 true 分支是立即函数上下文；
`if (std::is_constant_evaluated()) { call_consteval(x); }` 非法，因为两分支按普通代码检查，`x` 不是常量表达式。

---

## 5. 本次改动

| 动作 | 说明 |
|------|------|
| 全量交叉审读 | 21 文件均有 static_assert/assert 双路径；专家段覆盖 UB/工具链 |
| 代码改动 | **无强制修改**（覆盖已达标；stage05 侧 constexpr unique_ptr 已与本 stage 呼应） |

---

## 6. 残留

- 跨 TU SIOF：单文件只能**模拟语义**，真实链接顺序 fiasco 需两 .cpp（文档扩展练习 4）。
- MSVC `cl` 对部分 constexpr unique_ptr/array 常量求值仍弱；本仓库 **clang-cl -std=c++23** 为准。
- cmath/from_chars 子集：实现参差，文件内已降级说明。

---

## 7. 验收清单对照（文档 11.1）

| 验收项 | 覆盖 |
|--------|------|
| constexpr + static_assert | ✅ `static_assert` / `constexpr_function_*` |
| constexpr vector + unique_ptr | ✅ section05 两文件 |
| if consteval 双路径 + 能调 consteval | ✅ section02 两文件 |
| constexpr from_chars | ✅ `constexpr_to_chars_from_chars_cpp23` |
| constexpr / consteval / constinit 三者 | ✅ section01+03 注释表 + 各专题 |

**Stage07 结论：验收点全覆盖；if consteval 与 ice 区别为 STRONG；R2 通过。**
