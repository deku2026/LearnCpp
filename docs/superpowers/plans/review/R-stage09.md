# R-stage09 · 异常 / 错误处理 / UB 纪律 — 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Wave:** R3
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段9-异常-错误处理与UB纪律.md`
**Cpp root:** `src/cpp/part2_stage09_exceptions_error_handling_ub/`
**Topic count:** 45

> 注意：本 stage 若干 UB/诊断文件原先偏 **thin**（~50 行入门层），本轮抬升。

---

## 1. 文档概念清单 L（权威）

| # | 文档锚点 | 概念 | 验收/重点 |
|---|----------|------|-----------|
| L1 | 步骤 1 | try/catch/throw；exception 层次；栈展开+RAII；`throw;` | const& 捕获、重抛 |
| L2 | 步骤 2 | noexcept 说明符/运算符；移动为何 noexcept | vector growth / move_if_noexcept |
| L3 | 步骤 3 | 四级异常安全 + copy-and-swap | 强保证赋值 |
| L4 | 步骤 4 | `std::expected` + monadic 铁路式 | and_then/transform/or_else/transform_error |
| L5 | 步骤 5 | optional monadic（C++23） | transform/and_then/or_else |
| L6 | 步骤 6 | variant 错误；error_code/system_error | 模式分发 / ABI 边界 |
| L7 | 步骤 7 | 错误处理选型四维表 | expected vs 异常 vs optional vs ec |
| L8 | 步骤 8 | stacktrace / unreachable / print | C++23 诊断 |
| L9 | 步骤 9–10 | scope guard rollback；析构不得抛 | dismiss / noexcept dtor |
| L10 | 步骤 11 | UB 意识 + 清单 + bit_cast + Sanitizer | 不触发 UB 的对照写法 |
| L11 | 步骤 12–14 | Qt 对照 / UB 高频清单 / 验收 | 选型与护栏 |

---

## 2. Cpp 文件清单 F

| Section | # | 代表 items |
|---------|---|------------|
| section01_exception_mechanism | 4 | try_catch_throw_basics, std_exception_hierarchy, stack_unwinding_with_raii, rethrow_and_catch_all |
| section02_noexcept_basics | 3 | noexcept_specifier, noexcept_operator, move_should_be_noexcept_for_vector_growth |
| section03_exception_safety_levels | 5 | no/basic/strong/no_throw + copy_and_swap_strong_guarantee |
| section04_std_expected_cpp23 | 9 | basics…railway…versus…unexpected-taged |
| section05_non_exception_error_styles | 5 | optional monadic, error_code, system_error, variant |
| section06_diagnostics_cpp23 | 4 | stacktrace, unreachable, print, format_thread_id_stacktrace |
| section07_raii_rollback_and_destructor_rules | 3 | scope_guard, dtor must not throw, dtor default noexcept |
| section08_undefined_behavior | 12 | ub_general + 清单各项 + sanitizer_protection |

---

## 3. 覆盖矩阵 L × F

| L | 主要 cpp | 评级 | 备注 |
|---|----------|------|------|
| L1 | section01 ×4 | **PASS+** | 层次/RAII 展开/exception_ptr 重抛 |
| L2 | section02 ×3 | **STRONG** | vector 增长 + move_if_noexcept 叙事完整 |
| L3 | section03 ×5 | **STRONG** | copy_and_swap 最厚 |
| L4 | section04 ×9 | **STRONG** | 铁路式 + 选型文件；修 or_else 返回类型 |
| L5 | optional monadic | **PASS** | C++23 monadic 对照 expected |
| L6 | error_code / variant / system_error | **PASS** | |
| L7 | expected_versus_exception_optional_error_code | **PASS+** | 四风格同操作 + 决策表 |
| L8 | section06 ×4 | **PASS+** | print 结构化诊断段补强 |
| L9 | section07 ×3 | **PASS** | |
| L10 | section08 ×12 | **PASS → PASS+** | thin UB 文件补进阶层 |
| L11 | 注释/对照 | **PASS** | Qt 对照分散在注释 |

**FAIL:** 0　**WEAK 抬升:** out_of_bounds / null / UAF / ub_general / print / expected_transform；**编译修:** expected_or_else

---

## 4. 检索补细节

| 主题 | 标准/提案 | 写入位置 |
|------|-----------|----------|
| expected monadic | P2505 / [expected.object.monadic] | section04 全套 |
| or_else 必须返回 expected | N4950 monadic §15 | `expected_or_else` 修 bug |
| bad_expected_access | C++23 | `expected_value_and_error` |
| bit_cast 类型双关 | P0476 / [bit.cast] | `strict_aliasing_violation` |
| stacktrace | P0881 | `std_stacktrace_current_cpp23` |
| unreachable | P0627 | `std_unreachable_cpp23` |
| 有符号溢出检查反模式 | [expr.pre] UB | `signed_integer_overflow` / `ub_general` |

**安全纪律：** section08 一律「良定义对照 + 注释危险路径」，不故意执行 UB。

---

## 5. 本次改动

| 文件 | 变更 |
|------|------|
| `section04/.../expected_or_else.cpp` | **修编译**：`or_else` 回调返回 `expected{std::unexpected-tag,...}` |
| `section04/.../expected_transform.cpp` | 专家段：链式类型演化 int→string |
| `section06/.../std_print_for_diagnostics_cpp23.cpp` | 结构化诊断行 |
| `section08/.../out_of_bounds_access.cpp` | span.subspan、[] vs at 契约 |
| `section08/.../null_pointer_dereference.cpp` | 引用=非空契约 |
| `section08/.../use_after_free.cpp` | weak_ptr 观察寿命 |
| `section08/.../ub_general_concept.cpp` | UB 类别地图 + Sanitizer 分工 |

**编译：** 本 stage 改动 obj 以 `clang-cl -std=c++23` 通过（全量 link 被 stage10 无关错误挡住，见 R7）。

---

## 6. 残留

- 文档步骤 12 Qt 对照无独立 topic（工程选择：注释 + expected_versus 覆盖选型）。
- Sanitizer 真实报警需 Linux/本地 ASan 作业，topic 内不触发。
- `std::stacktrace` 符号解析依赖实现/链接器配置。

---

## 7. 验收清单对照（文档 14.1）

| 验收项 | 覆盖 |
|--------|------|
| copy-and-swap 强保证 | ✅ `copy_and_swap_strong_guarantee` |
| 移动为何 noexcept | ✅ `move_should_be_noexcept_for_vector_growth` |
| expected 铁路式管道 | ✅ `railway_oriented_pipeline` + monadic 族 |
| stacktrace 打印 | ✅ `std_stacktrace_current_cpp23` |
| optional monadic | ✅ `optional_and_then_transform_or_else_cpp23` |
| strict aliasing + bit_cast | ✅ `strict_aliasing_violation` |

**Stage09 结论：验收点全覆盖；thin UB/诊断抬升；or_else 编译缺陷已修；R3 通过。**
