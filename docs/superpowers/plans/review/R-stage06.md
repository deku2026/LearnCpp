# R-stage06 · 模板与泛型编程 — 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Wave:** R2
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段6-模板与泛型编程.md`
**Cpp root:** `src/cpp/part2_stage06_templates_and_generics/`
**Topic count:** 40

---

## 1. 文档概念清单 L（权威）

| # | 文档锚点 | 概念 | 验收/重点 |
|---|----------|------|-----------|
| L1 | 步骤 1 | 函数模板；单态化；鸭子类型 | 手写 max；推导与 auto 同源 |
| L2 | 步骤 2 | 类模板；成员模板；按需实例化 | Stack；定义放头文件心智 |
| L3 | 步骤 3 | 类型 / NTTP / 模板模板 / 默认参数 | C++20 字面值类 NTTP；auto NTTP |
| L4 | 步骤 4 | 推导细节：按值/引用/转发；不参与推导位置 | 数组保留维度等 |
| L5 | 步骤 5 | 全特化 / 偏特化 / 显式实例化 | 函数模板不能偏特化 |
| L6 | 步骤 6 | 别名模板 | |
| L7 | 步骤 7 | 参数包、sizeof...、折叠、可变完美转发 | 四种折叠；make 风格工厂 |
| L8 | 步骤 8 | CTAD、推导指引、继承构造 CTAD P2582 | C++23；工具链 fallback |
| L9 | 步骤 9 | type_traits / SFINAE / enable_if / void_t / tag dispatch | |
| L10 | 步骤 10 | `if constexpr` vs SFINAE；`if consteval` 链接 | 验收：两版 half |
| L11 | 步骤 11 | Concepts：定义、requires 子句/表达式、四类要求、标准库、替代 SFINAE、subsumption | 报错质量对比 |
| L12 | 步骤 12 | CRTP 静态多态；deducing this 简化；vs 虚函数 | 两版对比验收 |
| L13 | 步骤 13 | TMP 数值/类型计算；表达式模板了解 | 递归实例化 |

---

## 2. Cpp 文件清单 F

| Section | Files (count) |
|---------|----------------|
| section01_template_basics | 10：`function_template`, `class_template`, `member_template`, `non_type_template_parameter`, `template_template_parameter`, `default_template_arguments`, `template_argument_deduction`, `full_template_specialization`, `partial_template_specialization`, `explicit_template_instantiation` |
| section02_variadic_templates | 5：`parameter_packs`, `pack_expansion`, `sizeof_pack`, `fold_expressions_cpp17`, `variadic_perfect_forwarding` |
| section03_type_deduction_advanced | 5：`auto_nttp_cpp17`, `alias_template_advanced`, `ctad_*`, `deduction_guides`, `inherited_ctor_ctad_cpp23` |
| section04_compile_time_introspection_traditional | 7：`type_traits_*`, `sfinae_*`, `enable_if`, `void_t`, `tag_dispatch`, `if_constexpr_cpp17`, `if_consteval_cpp23_link` |
| section05_concepts | 6：`concept_definition`, `requires_clause`, `requires_expression`, `standard_concepts_library`, `replace_sfinae_with_concepts`, `constraint_subsumption_partial_order` |
| section06_crtp | 4：`crtp_static_polymorphism`, `crtp_inject_behavior`, `crtp_versus_virtual`, `crtp_deducing_this_simplification_cpp23` |
| section07_tmp_intro | 3：`recursive_instantiation`, `compile_time_type_calculation`, `expression_templates_intro` |

---

## 3. 覆盖矩阵 L × F

| L | 主要 cpp | 评级 | 备注 |
|---|----------|------|------|
| L1 | `function_template`, `template_argument_deduction` | **STRONG** | 多模式推导 + Insights 导向注释 |
| L2 | `class_template`, `member_template` | **STRONG** | Stack 级完整示例；成员模板 push 风格 |
| L3 | `non_type_template_parameter`, `template_template_parameter`, `default_template_arguments`, `auto_nttp_cpp17` | **STRONG** | 含 C++20 `Scale` 字面值类 NTTP；指针 NTTP |
| L4 | `template_argument_deduction` | **PASS+** | 三种基本模式齐全 |
| L5 | `full_*`, `partial_*`, `explicit_*` | **PASS** | 函数不能偏特化有注释/对照 |
| L6 | `alias_template_advanced` | **PASS** | |
| L7 | section02 全部 | **STRONG** | 折叠四形态 + 完美转发工厂 |
| L8 | `ctad_*`, `deduction_guides`, `inherited_ctor_ctad_cpp23` | **PASS** | P2582：显式 guide 作 fallback（工具链参差） |
| L9 | section04 传统内省 | **STRONG** | void_t 探测、tag dispatch、enable_if |
| L10 | `if_constexpr_cpp17`, `if_consteval_cpp23_link` | **STRONG** | SFINAE half 双写 vs if constexpr；链接 stage07 |
| L11 | section05 全部 | **STRONG** | 四类 requires；四种约束写法；subsumption |
| L12 | section06 全部 | **STRONG** | 传统 CRTP / 注入 Comparable / vs virtual / deducing this |
| L13 | section07 全部 | **PASS** | 表达式模板为了解级，已含朴素 vs 延迟对照 |

**FAIL:** 0　**WEAK:** 0（抽检后均为 PASS+）

---

## 4. 检索补细节（Concepts / 模板）

| 主题 | 标准/提案 | 写入位置 |
|------|-----------|----------|
| 四类 requirement | [expr.prim.req] | `requires_expression` |
| 四种约束施加写法 | [temp.pre] / [temp.constr] | `requires_clause` |
| subsumption 偏序 | [temp.constr.order] | `constraint_subsumption_partial_order` |
| SFINAE → Concepts 迁移 | [temp.deduct] / constraints | `replace_sfinae_with_concepts` |
| 折叠表达式四形态 | P0036 / [temp.variadic] | `fold_expressions_cpp17` |
| CTAD + P2582 继承构造 | P2582R1 | `inherited_ctor_ctad_cpp23`（guide fallback） |
| C++20 class NTTP | P0732 等 | `non_type_template_parameter` |
| if constexpr 丢弃非法分支 | [stmt.if] | `if_constexpr_cpp17` |

**注：** Concepts 报错「清晰度」无法在 assert 中机器验证；文件用注释标出「constraints not satisfied」对照 SFINAE 长串 substitution，验收依赖读者本地触发编译错误。

---

## 5. 本次改动

| 动作 | 说明 |
|------|------|
| 全量交叉审读 | 40 文件无空壳/无 `assert(true)` 占位；入门/进阶/专家分层齐全 |
| 代码改动 | **无强制修改**（覆盖已达标）；与 stage05 联动的 `if_consteval_cpp23_link` 保持为 stage07 入口 |

---

## 6. 残留

- `inherited_ctor_ctad_cpp23`：依赖实现；以显式 deduction guide 保证可编译，专家段说明 P2582 语义。
- 表达式模板：故意保持「了解级」深度，深水见 part6 branch E。
- Qt/`QVariant` 对照：文档步骤 14，cpp 不单开 topic。

---

## 7. 验收清单对照（文档 16.1）

| 验收项 | 覆盖 |
|--------|------|
| 函数/类/可变参数模板各一例 | ✅ section01 + section02 |
| `if constexpr` vs SFINAE | ✅ `if_constexpr_cpp17` |
| concept 约束 + 报错对比（注释） | ✅ `replace_sfinae_with_concepts` |
| CRTP 两版 + vs 虚函数 | ✅ section06 |
| C++ Insights 看实例化 | ✅ 各文件注释引导 |

**Stage06 结论：文档概念全覆盖；无 FAIL/WEAK；R2 通过。**
