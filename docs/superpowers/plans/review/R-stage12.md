# R-stage12 · 协程 — 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Wave:** R3
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段12-协程.md`
**Cpp root:** `src/cpp/part2_stage12_coroutines/`
**Topic count:** 32

> 对照 Boost.Asio / Coroutine2 以**教学注释**写入，不强制链接依赖。

---

## 1. 文档概念清单 L（权威）

| # | 文档锚点 | 概念 | 验收/重点 |
|---|----------|------|-----------|
| L1 | 步骤 1 | 协程定义；stackless；挂起/恢复/销毁 | 三关键字识别 |
| L2 | 步骤 2 | 编译器改写；协程帧；参数进帧；HALO | by-ref 悬垂根源 |
| L3 | 步骤 3 | promise_type 钩子全家桶 | initial/final/yield/return/exception |
| L4 | 步骤 4 | Awaitable/Awaiter 三件套；对称转移 | await_suspend 返回 handle |
| L5 | 步骤 5 | coroutine_handle | resume/destroy/done |
| L6 | 步骤 6 | 一次 co_await 全流程 | 验收讲解点 |
| L7 | 步骤 7 | 手写 Generator | 看穿原理 |
| L8 | 步骤 8 | std::generator + view + elements_of | C++23 主线 |
| L9 | 步骤 9–11 | 手写 Task；对称转移防栈溢；定位边界 | async 标准未提供 |
| L10 | 步骤 12 | 坑：by-ref、帧分配、dtor 非协程、寿命 | UB 清单 |

---

## 2. Cpp 文件清单 F

| Section | # | 代表 items |
|---------|---|------------|
| section01_foundations | 5 | what_is, stackless, frame_layout, co_*, by_value_vs_ref |
| section02_promise_type | 7 | intro + 各钩子 |
| section03_awaitable_and_awaiter | 5 | ready/suspend/resume, suspend_always/never, await_transform |
| section04_coroutine_handle | 3 | basics, resume_destroy_done, scheduling intro |
| section05_std_generator_cpp23 | 5 | basic, as_view, elements_of, vs handwritten, async_task_not_provided |
| section06_hand_rolled_examples | 3 | handwritten generator/task, co_await chained |
| section07_pitfalls | 4 | by_ref dangle, frame alloc, dtor, lifetime |

---

## 3. 覆盖矩阵 L × F

| L | 主要 cpp | 评级 | 备注 |
|---|----------|------|------|
| L1 | section01 | **PASS+** | |
| L2 | frame_layout + by_value_vs_ref + frame_allocation | **STRONG** | HALO 注释 |
| L3 | section02 ×7 | **STRONG** | 钩子拆文件 |
| L4 | section03 ×5 | **STRONG** | 对称转移 180 行级 |
| L5 | section04 | **PASS** | |
| L6 | await_ready/suspend + chained_resumption | **PASS+** | 流程拆解 |
| L7 | handwritten_generator | **PASS** | |
| L8 | section05 generator 族 | **STRONG** | elements_of 验收 |
| L9 | handwritten_task + async_task_not_provided | **STRONG** | Asio/Coroutine2 对照 |
| L10 | section07 | **PASS+** | |

**FAIL:** 0　**WEAK 抬升:** async 边界与第三方对照注释

---

## 4. 检索补细节

| 主题 | 标准/提案 | 写入位置 |
|------|-----------|----------|
| 协程语言机制 | N4736 / P0912 / [dcl.fct.def.coroutine] | foundations + promise |
| HALO | P0981 | frame_layout / frame_allocation |
| std::generator | P2502 | section05 |
| elements_of | generator recursive yield | `elements_of_recursive_yield` |
| symmetric transfer | Lewis Baker 系列 | await_suspend + handwritten_task |
| std::execution 方向 | P2300 | async_task_not_provided 注释 |
| Boost.Asio awaitable | 外部库 | 教学注释（无链接） |
| Boost.Coroutine2 stackful | 外部库 | 与 stackless 代际对照 |

---

## 5. 本次改动

| 文件 | 变更 |
|------|------|
| `section05/.../async_task_not_provided_by_std.cpp` | 展开 Asio/Coroutine2/C++26 execution 教学对照段 |
| `section06/.../handwritten_task_with_symmetric_transfer.cpp` | 文件头补充 Asio/Coroutine2 对照注释 |

**编译：** 改动 obj 通过。

---

## 6. 残留

- 无真实 io_context/co_spawn 可运行示例（刻意不引 Boost 依赖）。
- godbolt/cppinsights 看状态机：文档作业，非 assert 可验。
- C++26 `std::execution` 仅定位，不伪实现。

---

## 7. 验收清单对照（文档 15.1）

| 验收项 | 覆盖 |
|--------|------|
| co_await 全流程讲解 | ✅ awaiter 三件套 + chained_resumption |
| std::generator + ranges | ✅ `std_generator_as_view` |
| elements_of 递归 | ✅ `elements_of_recursive_yield` |
| 手写 Generator 对照 | ✅ handwritten + standard_versus |
| 手写 Task + 对称转移 | ✅ `handwritten_task_with_symmetric_transfer` |
| by-ref 悬垂 | ✅ `by_reference_parameter_dangling` |

**Stage12 结论：原理线 + generator 验收 + 第三方对照注释齐；R3 通过。**
