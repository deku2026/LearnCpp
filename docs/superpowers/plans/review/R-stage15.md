# R-stage15 · C++ 经典惯用法 — 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Wave:** R3
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段15-C++经典惯用法.md`
**Cpp root:** `src/cpp/part2_stage15_classic_idioms/`
**Topic count:** 22 → **24**（本轮 +2）

---

## 1. 文档概念清单 L（权威）

| # | 文档锚点 | 概念 | 验收/重点 |
|---|----------|------|-----------|
| L1 | 步骤 1 | RAII / Rule of 0·3·5 / scope guard / copy-and-swap / pimpl / erase-remove | 资源寿命族 |
| L2 | 步骤 2 | NVI / CRTP(+deducing this) / 类型擦除 / tag dispatch | 多态与接口 |
| L3 | 步骤 3 | detection / SFINAE·if constexpr·if consteval / policy / CPO / 表达式模板 | 泛型编译期 |
| L4 | 步骤 4 | =delete / strong typedef / builder / EBO·no_unique_address / optional·expected / 递归 lambda | 值语义工具 |
| L5 | 步骤 5 | **综合改写：旧代码→现代惯用法** | 验收点（曾缺独立 topic） |
| L6 | 步骤 6–7 | Qt 惯用法对照；**何时不用**矩阵 | 防过度设计 |
| L7 | 步骤 8 | 验收清单 | 见 §7 |

---

## 2. Cpp 文件清单 F

| Section | # | items |
|---------|---|-------|
| section01_resource_lifetime | 6 | raii, rule_of_0_3_5, scope_guard, copy_and_swap, pimpl, erase_remove |
| section02_polymorphism_and_interface | 4 | nvi, crtp(+deducing this), type_erasure, tag_dispatch |
| section03_generic_and_compile_time | 6 | detection, sfinae_if_constexpr, if_consteval, policy, cpo, expression_template |
| section04_value_semantics_and_tools | 6→**8** | delete, strong_typedef, builder, ebo, optional_expected, recursive_lambda, **legacy_to_modern_rewrite**, **idiom_when_to_use_matrix** |

---

## 3. 覆盖矩阵 L × F

| L | 主要 cpp | 评级 | 备注 |
|---|----------|------|------|
| L1 | section01 ×6 | **PASS+** | erase-remove 含 C++20 erase_if |
| L2 | section02 ×4 | **STRONG** | CRTP 新旧对照 + virtual 对比 |
| L3 | section03 ×6 | **PASS+** | detection/CPO/policy 齐全 |
| L4 | section04 原 6 文件 | **PASS+** | |
| L5 | **legacy_to_modern_rewrite**（新） | **STRONG** | 文档 5.1–5.3 完整落地 |
| L6 | **idiom_when_to_use_matrix**（新） | **PASS+** | 步骤 7 表 + 微探针 |
| L7 | 全 stage | **PASS** | |

**FAIL:** 0　**MISSING→补全:** 步骤 5 综合改写、步骤 7 何时不用

---

## 4. 检索补细节

| 主题 | 来源 | 写入位置 |
|------|------|----------|
| copy-and-swap 强保证 | Exceptional C++ / Core Guidelines | `copy_and_swap_idiom` |
| pimpl ABI 防火墙 | Sutter | `pimpl_idiom` |
| CRTP → deducing this | C++23 | `crtp_idiom` |
| type erasure vs variant | 文档 2.3 | `type_erasure_idiom` |
| detection → Concepts | void_t 时代 vs requires | `detection_idiom` |
| CPO 两步 | Niebler / [customization.point] | `cpo_customization_point` |
| 旧→新改写表 | 文档 5.3 | `legacy_to_modern_rewrite` |
| 何时不用 | 文档步骤 7 | `idiom_when_to_use_matrix` |

---

## 5. 本次改动

| 文件 | 变更 |
|------|------|
| `section04/.../legacy_to_modern_rewrite.cpp` | **新增**：unique_ptr + expected + RAII 铁路改写验收 |
| `section04/.../idiom_when_to_use_matrix.cpp` | **新增**：何时不用表 + strong typedef/optional/builder 探针 |

**编译：** 新文件 obj 通过。

---

## 6. 残留

- pimpl 真拆 .h/.cpp 多 TU 演示：单 topic 内嵌 Impl，完整双文件作业留给读者。
- 表达式模板仅「了解级」微例（文档定位一致）。
- Qt 惯用法对照以注释/选型表呈现，无 Qt 依赖。

---

## 7. 验收清单对照（文档 8.1）

| 验收项 | 覆盖 |
|--------|------|
| 五惯用法各一小例子 | ✅ section01–03 对应文件 |
| deducing this 改写 CRTP + 递归 lambda | ✅ `crtp_idiom` + `recursive_lambda_via_deducing_this_idiom` |
| 每惯用法「何时不用」 | ✅ `idiom_when_to_use_matrix` |
| 旧代码→现代改写 | ✅ `legacy_to_modern_rewrite` |

**Stage15 结论：原 22 惯用法 PASS+；补文档验收双缺口；R3 通过。**
