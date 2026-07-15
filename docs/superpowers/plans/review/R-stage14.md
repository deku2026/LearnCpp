# R-stage14 · 特性与关键字全表 — 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Wave:** R3
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段14-特性与关键字全表.md`
**Cpp root:** `src/cpp/part2_stage14_keywords_and_features_index/`
**Topic count:** 18

> 本 stage 是**清单/对账**型：不以「每特性独立深讲」为目标（深讲在 stage1–13）；以 feature-test + 微探针覆盖为权威。

---

## 1. 文档概念清单 L（权威）

| # | 文档锚点 | 概念 | 验收/重点 |
|---|----------|------|-----------|
| L1 | 步骤 1 | 关键字全表 + 上下文关键字 | override/final/requires/module… |
| L2 | 步骤 2 | Feature-test 宏 `__cpp_*` / `__cpp_lib_*` | 对账工具 |
| L3 | 步骤 3 | A–P 特性分类架 | 16 桶导航 |
| L4 | 步骤 4 | 按标准清单 C++11→23 | 每标准微矩阵 |
| L5 | 步骤 5 | C++23 签名特性 | deducing this / expected / print / … |
| L6 | 步骤 6 | C++23 库细补丁 | allocate_at_least 等「只在表里」项 |
| L7 | 步骤 7 | 移除/弃用/取消弃用/C 兼容头 | 迁移敏感 |
| L8 | 步骤 8–10 | Qt 速查 / 易混 / 验收对账法 | 一页速查 |

---

## 2. Cpp 文件清单 F

| Section | # | items |
|---------|---|-------|
| section01 cpp11 | 2 | language + library overview |
| section02 cpp14 | 2 | language + library overview |
| section03 cpp17 | 2 | language + library overview |
| section04 cpp20 | 2 | language + library overview |
| section05 cpp23 language | 1 | language feature checklist |
| section06 cpp23 library | 1 | library feature checklist（含 allocate_at_least） |
| section07 keyword_table | 1 | keyword_taxonomy_a_to_p |
| section08 deprecations | 4 | deprecated / removed / undeprecation / C compat headers |
| section09 feature_test | 3 | lang macros / lib macros / has_include+attribute |

---

## 3. 覆盖矩阵 L × F

| L | 主要 cpp | 评级 | 备注 |
|---|----------|------|------|
| L1 | keyword_taxonomy_a_to_p | **PASS+** | A–P 微探针 + 上下文关键字 |
| L2 | section09 ×3 | **STRONG** | 宏清单输出 |
| L3 | keyword_taxonomy | **PASS** | 与步骤 3 同构 |
| L4 | section01–04 | **PASS+** | 各标准微矩阵 |
| L5 | section05 + 06 | **STRONG** | feature-test 门控 |
| L6 | cpp23_library + lib macros | **PASS** | allocate_at_least 有探针 |
| L7 | section08 ×4 | **PASS** | |
| L8 | 注释/宏输出 | **PASS** | 速查由宏清单承担 |

**FAIL:** 0　**WEAK 抬升:** cpp14_library（quoted + feature-test 锚）

---

## 4. 检索补细节

| 主题 | 标准/来源 | 写入位置 |
|------|-----------|----------|
| Feature testing | cppreference feature_test | section09 |
| Compiler support 对账法 | compiler_support/23 | 文档方法；宏运行时打印 |
| allocate_at_least | P0401 | `cpp23_library_feature_checklist` |
| C++23 签名九件套 | 文档步骤 5 | language + library checklists |
| 弃用/移除 | 文档步骤 7 | section08 |
| C 兼容头版本化 | C++23 | `new_c_compat_headers` |

---

## 5. 本次改动

| 文件 | 变更 |
|------|------|
| `section02/.../cpp14_library_features_overview.cpp` | `std::quoted` 往返 + `__cpp_lib_make_unique` 锚；补 iomanip/sstream/version |

**编译：** 改动 obj 通过。

---

## 6. 残留

- 无法在单 TU 演示「全量 ~97 关键字」；taxonomy 按类采样。
- Modules 关键字仅宏/注释（stage13 不做）。
- 部分 C++23 库项在旧 STL 上 SKIP 打印（feature-test 降级正确）。

---

## 7. 验收清单对照（文档 10.1）

| 验收项 | 覆盖 |
|--------|------|
| 清单能写小例子 | ✅ 各 overview/checklist 微探针 |
| Feature-test 对账 | ✅ section09 |
| 一页速查心智 | ✅ A–P taxonomy + 宏输出 |
| 背出 C++23 签名特性 | ✅ section05/06 |
| 表内细补丁不漏 | ✅ allocate_at_least 等 |

**Stage14 结论：对账型 stage 覆盖完整；R3 通过。**
