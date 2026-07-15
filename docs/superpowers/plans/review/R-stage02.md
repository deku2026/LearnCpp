# R-stage02 · 类型系统 / 值类别 / 类型推导 · 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Worktree:** `C:\MyFile\ArcForges\LearnCpp\.worktrees\fill-cpp-examples`
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段2-类型系统-值类别与类型推导.md`
**Code:** `src/cpp/part2_stage02_type_system_value_category_deduction/` (53 `.cpp`)

---

## 1. 文档清单 L（步骤 / 概念 / 验收 / UB / C++23）

| ID | 锚点 | 要点 |
|----|------|------|
| L0 | §0 | `static_assert`+`is_same_v`；TypeDisplayer；`typeid` 慎用 |
| L1 | §1 | 整型/字符家族；`<cstdint>`；`<stdfloat>`；`0uz`/`z`；`nullptr` |
| L2 | §2 | 六种初始化；`{}` 窄化；`initializer_list` 劫持；MVP；static 零初始化；未初始化 UB |
| L3 | §3 | 顶层/底层 const；const&；`constexpr` 变量；`volatile≠线程同步` |
| L4 | §4 | 左值/右值引用；指针对照；引用折叠；转发引用 vs `T&&` |
| L5 | §5 | lvalue/xvalue/prvalue；glvalue/rvalue；C++17 prvalue/物化；C++23 隐式移动预告 |
| L6 | §6 | auto 剥顶层；`decltype`/`(x)`；`decltype(auto)`；`auto(x)` decay-copy |
| L7 | §7 | `using` vs typedef；别名模板；type_traits 入门 |
| L8 | §8 | 裸 enum 问题；`enum class`；底层类型；`using enum`；`to_underlying`/`is_scoped_enum` |
| L9 | §9 | struct/聚合；designated init；括号聚合了解 |
| L10 | §10 | `\N{}`；`\u{}`/`\x{}`/`\o{}`；UTF-8 源 |
| L11 | §11 | Qt COW vs 移动语义（验收） |
| L12 | §12 | UB 清单 16 条 |
| L13 | §13 | 验收：值类别/auto/decltype((x))/list 劫持/MVP/`auto(x)`/`0uz`/enum class/Qt |

---

## 2. 文件清单 F（53，按 section）

| Section | 数量 | 代表 item |
|---------|------|-----------|
| section01_builtin_types_and_initialization | 8 | integral…, cstdint, stdfloat, 0uz, nullptr, braces, auto_with_init, narrowing |
| section02_initialization_taxonomy | 10 | default/value/direct/copy/list/aggregate, hijack, MVP, zero_init, uninit_ub |
| section03_const_constexpr_volatile | 4 | const ptr/ref, top/low const, constexpr, volatile |
| section04_compound_types | 4 | pointer, lvalue_ref, rvalue_ref, declaration_parsing |
| section05_value_categories | 4 | lvalue_xvalue_prvalue, glvalue_and_rvalue, prvalue_materialization, implicit_move_cpp23 |
| section06_type_deduction | 8 | auto_decays, decltype*, auto_decay_copy, collapse, move, forward, template_deduction |
| section07_type_aliases_and_traits | 3 | using, alias_template, type_traits |
| section08_enums | 6 | legacy, enum_class, underlying, using_enum, to_underlying, is_scoped_enum |
| section09_aggregates_and_struct_basics | 3 | struct, aggregate_rules, designated |
| section10_character_sets_cpp23 | 3 | named escape, brace escape, utf8 source |

---

## 3. 覆盖矩阵摘要

| L | 初评 | 终评 | 备注 |
|---|------|------|------|
| L0 实证工具 | WEAK（仅注释） | **PASS** | `decltype_basics` 写入 TypeDisplayer/typeid 纪律 |
| L1 类型/C++23 字面值 | PASS | PASS | feature-test 完备 |
| L2 初始化全家桶 | PASS；hijack/MVP 略薄 | **PASS+** | 两阶段 list 规则；MVP 与 hijack 分工 |
| L3 cv | PASS | PASS | volatile+atomic 对照扎实 |
| L4 引用/折叠 | PASS；rvalue 有 `\|\| true` | **PASS+** | 去掉空壳；补 Qt COW 对照 |
| L5 值类别 | PASS 缺探针 | **PASS+** | category_probe；物化「deferred materialization」 |
| L6 推导 | PASS；全家福不全 | **PASS+** | auto 全家福 static_assert |
| L7–L10 | PASS | PASS | 未强制改动 |
| L11 Qt COW | missing 专文代码 | **covered** | 挂在 `rvalue_reference` 专家段 |
| L13 验收 | 分散 PASS | **PASS+** | 关键验收点均有断言 |

**无空洞修复：**

- `pointer_basics`: `assert(**pp == x || true)` → 真实断言
- `rvalue_reference`: `assert(... || true)` → `clear()` 后 `empty` + 内容转移断言

---

## 4. 本轮改动文件（9）

1. `section02_initialization_taxonomy/braces_vs_parens_initializer_list_hijack.cpp`
   - 写入 list-initialization **两阶段 / 不回退** 规则；强化 size 验收断言。
2. `section02_initialization_taxonomy/most_vexing_parse_and_brace_fix.cpp`
   - 对象类型 vs 函数类型 `static_assert`；与 list 劫持分工说明。
3. `section04_compound_types/pointer_basics.cpp`
   - 去掉 `|| true` 空壳断言。
4. `section04_compound_types/rvalue_reference.cpp`
   - 移动后状态可移植检查；**Qt 隐式共享 vs 标准移动**验收文案+断言。
5. `section05_value_categories/lvalue_xvalue_prvalue.cpp`
   - 转发引用 `category_probe`（文档扩展练习 1）；≥6 类表达式左右值断言。
6. `section05_value_categories/glvalue_and_rvalue.cpp`
   - 补 `#include <type_traits>`；使用 `lr` 断言消 warning。
7. `section05_value_categories/prvalue_materialization_cpp17.cpp`
   - 「deferred temporary materialization」精确表述；const& 延长寿命示例。
8. `section06_type_deduction/auto_deduces_decays.cpp`
   - 对 `const int&` 源的 **auto / auto& / const auto& / auto&& / decltype / decltype((e)) / decltype(auto)** 全家福。
9. `section06_type_deduction/decltype_basics.cpp`
   - 步骤 0 TypeDisplayer / typeid 纪律写进专家段。

---

## 5. 检索补强要点

- [Value categories](https://en.cppreference.com/cpp/language/value_category)：三分 + glvalue/rvalue；History C++17。
- [Copy elision](https://en.cppreference.com/cpp/language/copy_elision)：prvalue 直接初始化；非「先有临时再 elide」。
- [List-initialization](https://en.cppreference.com/cpp/language/list_initialization)：`initializer_list` 优先且失败不回头。
- [decltype](https://en.cppreference.com/cpp/language/decltype) / [auto](https://en.cppreference.com/cpp/language/auto) / [explicit cast `auto(x)`](https://en.cppreference.com/cpp/language/explicit_cast)。
- 提案线索：P0849（auto(x)）、P2266（implicit move）、P0330（z/uz）、P1467（stdfloat）。

---

## 6. 抽检（PASS+）

| 文件 | 结果 |
|------|------|
| lvalue_xvalue_prvalue | PASS+（两问模型 + 探针 + 具名 &&） |
| prvalue_materialization_cpp17 | PASS+（NoCopyMove + 物化） |
| braces_vs_parens_initializer_list_hijack | PASS+（vector + Tracker + 两阶段） |
| auto_deduces_decays | PASS+（全家福） |
| decltype_basics / decltype_auto | PASS+ |
| std_forward_deduction_basis | PASS（未改；forward 代数完整） |
| auto_decay_copy_cpp23 | PASS（erase + inc_print 官方动机） |

**编译：** 本轮 9 个改动文件在 `windows-debug` / clang-cl C++23 下 **obj 编译通过**。全量链接仍受 stage09 无关错误影响。

---

## 7. 仍可接受的缺口

| 缺口 | 理由 |
|------|------|
| C++14 vs C++17 切换编译 NoCopyMove | 单预设 C++23；注释说明 C++14 会失败 |
| 真读未初始化触发 UBSan | 禁止故意 UB；`uninitialized_read_is_ub` 安全对照 |
| designated init 乱序 ill-formed 运行演示 | 只能注释「会编译失败」 |
| TypeDisplayer 真触发编译错误 | 会破坏 topic 可运行性；保留「自学写法」注释 |
| enum 位标志 `operator|` 扩展练习 | 非硬验收；可后续加分 |
| 独立 Qt COW 可运行 demo | 无 Qt 依赖；标准侧对照文案足够 |

---

## 8. 结论

- **FAIL：0**
- **WEAK 已抬升：** 空壳 assert、值类别探针、推导全家福、list 劫持两阶段、Qt 移动对照、TypeDisplayer 纪律。
- 文档 §13 硬验收点均能在对应 topic 中用 `assert`/`static_assert` 自证。
