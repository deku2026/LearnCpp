# R-stage11 · 并发与多线程 — 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Wave:** R3
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段11-并发与多线程.md`
**Cpp root:** `src/cpp/part2_stage11_concurrency_and_threading/`
**Topic count:** 35

---

## 1. 文档概念清单 L（权威）

| # | 文档锚点 | 概念 | 验收/重点 |
|---|----------|------|-----------|
| L1 | 步骤 1 | thread / jthread / thread_local | jthread + stop_token 优雅取消 |
| L2 | 步骤 2 | stop_token / source / callback | 协作取消；非强杀 |
| L3 | 步骤 3 | async / future / promise / packaged_task | 异步任务选型 |
| L4 | 步骤 4 | mutex 族 + RAII 锁 + shared_mutex | scoped_lock 防死锁 |
| L5 | 步骤 5 | condition_variable；谓词；生产者-消费者 | 有界队列 + 关闭 |
| L6 | 步骤 6 | latch / barrier / semaphore | C++20 协调原语 |
| L7 | 步骤 7 | atomic / CAS / flag / wait-notify / atomic_ref | 无锁计数 + CAS |
| L8 | 步骤 8 | memory_order 全家桶 | relaxed vs acq/rel vs seq_cst 选型 |
| L9 | 步骤 9 | 数据竞争=UB；volatile≠同步；C 原子互操作 | TSan 心智 |
| L10 | 步骤 10 | osyncstream；print 线程安全 | 输出交错消除 |

---

## 2. Cpp 文件清单 F

| Section | # | 代表 items |
|---------|---|------------|
| section01_threads_and_tasks | 6 | thread, jthread, jthread_stop_token, thread_local, async_future_promise, packaged_task |
| section02_synchronization_primitives | 13 | mutex/locks, CV, call_once, shared_mutex, latch/barrier/semaphores |
| section03_atomics_and_memory_model | 11 | atomic, CAS, orders, happens_before, volatile, atomic_ref, c_atomic |
| section04_cancellation_and_coordination | 3 | stop_token, graceful_shutdown, timed_wait |
| section05_output_thread_safety | 2 | osyncstream, print unicode thread safety |

---

## 3. 覆盖矩阵 L × F

| L | 主要 cpp | 评级 | 备注 |
|---|----------|------|------|
| L1 | section01 | **PASS+** | jthread 验收点完整 |
| L2 | stop_token + graceful_shutdown | **STRONG** | condition_variable_any 可中断 wait |
| L3 | async/promise/packaged_task | **PASS** | |
| L4 | mutex + 三锁 + shared_mutex | **PASS+** | 补 mutex 专家注记 |
| L5 | std_condition_variable | **STRONG** | 有界生产者-消费者 |
| L6 | latch/barrier/semaphores | **PASS** | barrier C++23 strengthening 单独文件 |
| L7 | std_atomic + compare_exchange_cas | **PASS+** | wait/notify、is_lock_free 补强 |
| L8 | memory_order_* ×4 + happens_before | **PASS+** | relaxed 专家段：无 SW 边 |
| L9 | data_race_is_ub_recap + volatile | **PASS** | |
| L10 | section05 ×2 | **PASS** | |

**FAIL:** 0　**WEAK 抬升:** std_mutex / memory_order_relaxed / std_atomic

---

## 4. 检索补细节

| 主题 | 标准/提案 | 写入位置 |
|------|-----------|----------|
| jthread RAII stop+join | P0660 / [thread.jthread] | `std_jthread_cpp20` |
| stop_token 协作取消 | 同上 | `stop_token_stop_source` |
| CV 谓词防虚假唤醒 | [thread.condition] | `std_condition_variable` |
| condition_variable_any + stop | C++20 | `graceful_shutdown` |
| memory_order | [atomics.order] | section03 全套 |
| atomic wait/notify | P1135 | `std_atomic` |
| atomic_ref | P0019 | `atomic_ref_cpp20` |
| C atomic interop | C++23 | `c_atomic_interop_cpp23` |
| osyncstream | P0053 | `osyncstream_cpp20` |

**平台：** TSan 非 Windows 默认路线；topic 内用 jthread 正确同步，数据竞争仅作「反例注释」。

---

## 5. 本次改动

| 文件 | 变更 |
|------|------|
| `section02/.../std_mutex.cpp` | 专家注：非递归、RAII 选型 |
| `section03/.../memory_order_relaxed.cpp` | 专家：无 synchronizes-with；适用/不适用 |
| `section03/.../std_atomic.cpp` | wait/notify 标注 7.4；is_lock_free 对照 |

**编译：** 改动 obj 通过。

---

## 6. 残留

- 无锁栈/ABA 深水在 part6_branch_f，本 stage CAS 文件点到为止。
- `memory_order_consume` 不推荐 → 未单独建 topic（文档亦弱化）。
- 真 TSan 抓竞争：Linux CI 作业，非本机 Windows 默认。

---

## 7. 验收清单对照（文档 13.1）

| 验收项 | 覆盖 |
|--------|------|
| jthread + stop_token 取消 | ✅ `jthread_stop_token` / `graceful_shutdown` |
| 生产者-消费者 | ✅ `std_condition_variable` |
| atomic 计数/标志 + order 选型 | ✅ `std_atomic` + memory_order_* |
| 数据竞争=UB | ✅ `data_race_is_ub_recap` |
| volatile 误区 | ✅ `volatile_is_not_synchronization` |

**Stage11 结论：五大主题与验收点全覆盖；内存序/atomic 补专家层；R3 通过。**
