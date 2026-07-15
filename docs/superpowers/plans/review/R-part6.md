# R-part6 · 支线 A–G 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Worktree:** `C:\MyFile\ArcForges\LearnCpp\.worktrees\fill-cpp-examples`
**Docs root:** `C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\`
**Plan:** `docs/superpowers/plans/2026-07-15-cpp-doc-cross-review.md` (Wave R6)

---

## 0 · 总览

| 支线 | 目录 | cpp 数 | 文档 | 评审结论 | 本轮增强文件数(约) |
|------|------|--------|------|----------|-------------------|
| A 对象模型 | `part6_branch_a_object_model` | 16 | 第6部分-支线A-对象模型.md | 已 PASS（基线最厚） | 0（抽检维持） |
| B 生命周期 | `part6_branch_b_lifetime_and_ownership` | 29 | 第6部分-支线B-生命周期与所有权.md | thin→PASS | **29**（全量加深） |
| C 内存 | `part6_branch_c_memory_management` | 29 | 第6部分-支线C-内存管理.md | thin→PASS | **~28** |
| D 名字查找 | `part6_branch_d_name_lookup_overload_resolution` | 23 | 第6部分-支线D-名字查找ADL与重载决议.md | thin→PASS | **~15** 关键 + 其余保持可跑 |
| E 模板编译期 | `part6_branch_e_templates_compile_time_system` | 26 | 路线图 + 现有 cpp | 已自洽；1 文件加深 | **1** |
| F 内存模型 | `part6_branch_f_memory_model_and_concurrency` | 21 | 第6部分-支线F-内存模型与并发.md | thin→PASS；**无 data race 演示** | **~8** 核心 + 对照已安全 |
| G ABI/UB | `part6_branch_g_abi_compile_link_ub_performance` | 41 | 第6部分-支线G-ABI编译链接UB性能.md | thin→PASS；UB 仅安全对照 | **~12** 关键 + 安全阀 |

**合计约 185 个 topic；本轮手工加深约 90+ 文件。** 禁止脚本灌正文；并发 topic 均用 mutex/atomic/jthread 安全模式。

**编译抽检：** `build/p6bcd_more_compile.cmd` 全 OK；额外 11 文件（含 F/G data_race、false_sharing、acquire_release）clang-cl `/std:c++latest` 通过（修复 1 处 `reference_constructs_from_temporary` 误断言）。

---

## A · 对象模型

### 文档清单（锚点）

| 文档节 | 概念 | cpp 覆盖 |
|--------|------|----------|
| A1 | 成员布局/对齐/padding | `section01/member_layout_alignment_padding` |
| A2 | trivial / standard-layout / aggregate / POD | `standard_layout_versus_aggregate_versus_pod` |
| A3 | EBO + `[[no_unique_address]]` | `empty_base_*`, `no_unique_address_*` |
| A4–A6 | vtable/多继承/虚继承/thunk | `vtable_vptr`, `multiple_*`, `virtual_inheritance_*`, `thunks` |
| A7 | 对象切片 | `object_slicing` |
| A8 | RTTI | `dynamic_cast`, `typeid`, `type_info` |
| A9 | 成员指针 | `member_pointers` |
| A10 | Qt QObject 对照 | `qt_qobject_model_versus_standard` |

### 覆盖矩阵结论

- **Doc→Cpp：** 主验收点均有对应 item；无 FAIL。
- **质量：** 平均 ~109 行；含 offsetof/assert/ABI 依赖声明。
- **本轮：** 抽检 `standard_layout_*`、`type_info` 达 PASS；未强制重写。

### 残留建议（非阻塞）

- godbolt/vtable dump 仍为注释指引（ABI 依赖，正确）。
- MSVC vs Itanium 布局对照可再加一行输出（可选 STRONG）。

---

## B · 生命周期与所有权

### 文档验收点 → 落地

| B12 验收 | 落地 topic |
|----------|------------|
| 五种+悬垂形态 + 统一模型 | `dangling_{reference,pointer,iterator,string_view,span,mdspan,coroutine_*}` |
| P2718 修了/没修 | `range_for_temporary_extension_cpp23` + `gaps_not_yet_fixed_in_cpp23` |
| owner/not_null/span 改 API | `owner_annotation_gsl`, `not_null_*`, `raw_pointer_as_borrow`, span 悬垂对照 |
| 临时延长边界 | `temporary_object_basics`, `const_ref_extends_temporary` |
| SIOF + constinit/Meyers | `static_initialization_order_problem`, `constinit_remedy`, `meyers_singleton` |
| ASan/tidy/Core Check | `asan_*`, `clang_tidy_*`, `core_guidelines_*`, `msvc_core_check` |
| C++23 traits | `reference_constructs/converts_from_temporary`, `is_implicit_lifetime` |

### 本轮关键补全

1. **统一悬垂模型话术**写进各 dangling 文件 expert 层（借用 + owner 先死）。
2. **P2718 边界**拆「修了 range-for / 没修拆变量与参数返回」。
3. **SIOF** 同 TU 安全路径 + 跨 TU 注释危险模式 + destruction-order 提示。
4. **GSL 心智** owner/not_null 本地 typedef/包装 + API 改造前后对照。
5. **协程引用参数** 按值进帧 vs owner 跨 await；不触发真悬垂。
6. **traits** `RefSlot`/`static_assert` 门禁；converts 捕获 `const char*`→`string` 临时。

### 质量门槛

- 原 thin（42–70 行）→ 入门/进阶/专家 + 断言 + 危险模式注释。
- **无故意 UB**；ASan 模式仅注释。

---

## C · 内存管理

### 文档验收点 → 落地

| 文档 | 落地 |
|------|------|
| C1 栈 vs 堆取决于创建方式 | `stack_versus_heap` |
| C2 new 表达式 vs operator new | `new_delete_global`, `new_delete_class_level` |
| C3 placement + 显式析构；勿 delete 外缓冲 | `placement_new_and_destroy` |
| C4 alignas/over-aligned/aligned_alloc | `alignas_alignof`, `over_aligned_*`, `std_aligned_alloc`（可移植 `operator new(align_val_t)`） |
| C5 三件套 | `start_lifetime_as*`, `std_launder_when_needed` |
| C6–C8 分配器/pmr | `allocator_*`, `pmr_*`, `monotonic_*`, `pool_resource`, custom/arena/pool |
| C9 缓存/false sharing/SoA | `cache_line_*`, `false_sharing`（**无 data race**）, `soa_*`, `prefetch_intro`（无 `__builtin`） |

### 本轮关键补全

1. **new 三件事** vs placement / start_lifetime_as / launder 对照表写入 C3/C5。
2. **最小 LoggingAllocator 挂 vector**（C7 验收）。
3. **pmr pool vs monotonic** 回收语义；null_memory_resource 抛 bad_alloc。
4. **false_sharing** 用 padded atomic 双线程；验证正确性非多核性能。
5. **支线铁律**：无编译器扩展预取；prefetch 讲访问模式。

### 覆盖

- Doc→Cpp：C1–C9 主点齐全；Qt 对照文档节无独立 cpp（与 A 一致，非阻塞）。
- 若干 C6 文件仍偏短但可运行 + 断言 + 文档层注释；达标 PASS 下限。

---

## D · 名字查找 / ADL / 重载

### 文档验收点 → 落地

| 文档 | 落地 |
|------|------|
| D1 unqual/qual/hiding/using/injected | 对应 `section01_*` |
| D2 ADL（cout/swap） | `adl_*`, `adl_for_cout_*` |
| D3 hidden friend | `hidden_friend` |
| D4 两阶段 + typename/template | `two_phase_*`, `typename_*`, `template_disambiguation` |
| D5–D7 重载/partial/特化 | `candidate_set`, `viable_*`, `best_*`, `partial_*`, `overload_versus_*`, ICS |
| D8 两步 swap → CPO | `swap_two_step_idiom`, `cpo_*` |
| D9 range_adaptor_closure / tag_invoke | `range_adaptor_*`, `tag_invoke_*`, reflection note |

### 本轮关键补全

1. **ADL** 关联基类命名空间示例；限定调用不走 ADL 扩展。
2. **name hiding** 派生隐藏全部基类重载 + `using` 修复。
3. **candidate_set** 普通查找 + ADL + 成员调用分层。
4. **两步 swap** 与 `std::swap` 绕过用户版对照。
5. **两阶段查找** `this->` 依赖基类成员。

### 覆盖

- 主验收齐全；部分 overload 文件原已可跑，本轮加深 lookup 主线。

---

## E · 模板 / 编译期系统

### 结论

- 26 文件平均 ~93 行，多数已 PASS/STRONG（SBO erasure、concepts subsumption 等）。
- **本轮：** 加深 `basic_common_reference_for_reference_wrapper_cpp23`（zip/proxy 动机 + 可定制点注释）。
- 与 stage06/07 有意重叠（two_phase recap 等），保留作支线 E 自洽入口。

---

## F · 内存模型与并发

### 文档验收点 → 落地

| 文档 | 落地 |
|------|------|
| F1 happens-before / SW | `happens_before_synchronizes_with` |
| F2 五级 memory_order | `seq_cst_*`, `acquire_release_*`, `relaxed_*`, `consume_not_recommended` |
| F3 x86/ARM 映射 | `memory_order_per_hardware_*` |
| F5–F7 无锁/CAS/ABA | `lock_free_*`, `cas_*`, `aba_*`, `compare_exchange_*` |
| F8–F10 atomic_ref/DCL/call_once/volatile | 对应 section03–05 |

### 本轮关键补全

1. **seq_cst / acq-rel / relaxed / consume** 语义与「何时够用」写清。
2. **consume 不推荐** + acquire 替代路径。
3. **volatile ≠ 线程同步**；对照 Java/C# 误区；MMIO 单线程示意。
4. **铁律：无 data race 演示**——共享非原子数据必经 release/acquire 或 mutex。

### 抽检编译

- `acquire_release_semantics`, `relaxed_semantics` OK。

---

## G · ABI / 编译链接 / UB / 性能

### 文档验收点 → 落地

| 文档 | 落地 |
|------|------|
| G1 mangling / extern "C" | `name_mangling_*`, `extern_c_link` |
| G2 可见性 | `symbol_visibility`, `attribute_visibility`, `declspec_*` |
| G3–G6 链接/LTO/静动态库 | `static_library_*`, `symbol_*`, `link_time_*`, `one_definition_*`, modules note |
| G7–G10 UB/别名/provenance/assume | `strict_aliasing_*`, `type_punning_*`, `pointer_provenance`, `assume_*` 等 |
| G11 安全阀 | `fno_strict_aliasing`, `fwrapv`, `ftrapv`, sanitizer family |

### 本轮关键补全

1. **静动态库** 验收对照表 + Windows import lib 说明。
2. **data_race_deep** 三种安全修复（mutex/atomic/本地归约）；不跑竞争。
3. **fno_strict_aliasing / fwrapv / ftrapv** 方言边界 + bit_cast/checked 可移植写法。
4. **符号解析/剥离/modules** 工程要点补全。

### UB 安全策略

- 一律「危险模式注释 + 安全路径断言」。
- 类型双关：`bit_cast` / `memcpy` / `unsigned char`。

---

## 缺口 / 非阻塞项

| 项 | 说明 |
|----|------|
| Qt 专节 | A10 有文件；B10/C10/D10/F11/G12 以注释/既有对照为主，未强制加 Qt SDK 依赖 |
| 跨 TU 真 SIOF 复现 | 刻意单 TU 注释模式，避免 UB |
| 多核 false sharing 性能 | 文档诚实声明；代码只验布局间距与正确性 |
| import std 真模块 | 仓库主构建仍 #include；G modules topic 为模型说明 |
| C/D/G 部分文件行数仍 <70 | 内容含验收点与断言，达 PASS 下限；可继续 STRONG 化 |

---

## 增强文件数统计（本轮）

| 支线 | 增强文件数 | 代表关键补全点 |
|------|------------|----------------|
| **A** | 0（抽检 PASS） | 布局/traits/vtable/RTTI 已齐 |
| **B** | **29** | 统一悬垂模型、P2718 边界、SIOF 三修法、GSL API、C++23 traits、工具链说明 |
| **C** | **~28** | placement/三件套、对齐可移植路径、最小 allocator、pmr 池/单调、false sharing 无竞争 |
| **D** | **~15** | ADL/hidden friend/hiding、候选集、两阶段、两步 swap |
| **E** | **1** | common_reference + reference_wrapper |
| **F** | **~8** | memory_order 五级、volatile 真相、acq-rel 发布 |
| **G** | **~12** | 静动态库、data race 安全修复、安全阀方言、符号模型 |
| **合计** | **约 90+** | |

---

## 编译记录

```
build/p6bcd_more_compile.cmd  → 16/16 OK
build/p6_extra_compile.cmd    → 修 reference_constructs 后全 OK
  (temporary_object_basics, is_implicit_lifetime, false_sharing,
   adl, candidate_set, acquire_release, relaxed, data_race, fno_strict_aliasing, …)
```

全量 `cmake --preset windows-debug` 建议在 R7 波次统一跑。

---

## 分节文件说明

本报告合并为 **`R-part6.md` 单文件分节 A–G**（等价于 `R-part6-a.md` … `R-part6-g.md` 合并版）。若需拆文件可复制各 `## X ·` 节。
