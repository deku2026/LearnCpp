# R-stage05 · 拷贝/移动语义与智能指针 — 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Wave:** R2
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段5-拷贝与移动语义-智能指针.md`
**Cpp root:** `src/cpp/part2_stage05_copy_move_smart_pointers/`
**Topic count:** 32

---

## 1. 文档概念清单 L（权威）

| # | 文档锚点 | 概念 | 验收/重点 |
|---|----------|------|-----------|
| L1 | 步骤 1.1–1.2 | 拷贝 vs 移动；资源类接管→源置空 | 写可移动资源类；解释移动构造三步 |
| L2 | 步骤 1.3 | valid-but-unspecified；自移动 | 无前置条件操作才可对 moved-from 使用 |
| L3 | 步骤 2 | `std::move` = static_cast 到右值 | const 退化拷贝；具名 && 是 lvalue |
| L4 | 步骤 3 | `noexcept` 移动 + `vector` 扩容 | `move_if_noexcept`；非 noexcept 成员拖累整类 |
| L5 | 步骤 4 | C++23 P2266 简化隐式移动 | move-eligible id 始终 xvalue；返回引用场景 |
| L6 | 步骤 5.1–5.2 | RVO / NRVO / C++17 保证消除 | prvalue 物化；不可移动类型返回 |
| L7 | 步骤 5.3 | 返回值别 `std::move`（F.48） | 阻止 NRVO；成员/参数才需 move |
| L8 | 步骤 6 | 转发引用 + `std::forward` + 包转发 | 漏 forward 永远走左值 |
| L9 | 步骤 7 | `std::forward_like`（P2445）+ deducing this | 模型对象值类别并入成员 |
| L10 | 步骤 8–9 | unique_ptr / 裸指针=借用 / 自定义删除器 | 派生→基需虚析构；**pImpl 不完整类型** |
| L11 | 步骤 9.6 | constexpr unique_ptr；out_ptr/inout_ptr | P2273 / C API 适配 |
| L12 | 步骤 10 | shared/weak 控制块、make_shared、循环、enable_shared_from_this | 一次分配；双控制块 UB |
| L13 | 步骤 11 | 临时寿命、const 引用延长、悬垂、移动后迭代器 | ASan 纪律 |

---

## 2. Cpp 文件清单 F

| Section | Files |
|---------|-------|
| section01_copy_vs_move (8) | `copy_construction`, `copy_assignment`, `move_construction`, `move_assignment`, `std_move_static_cast`, `valid_but_unspecified_state`, `noexcept_move_and_vector_growth`, `simplified_implicit_move_cpp23` |
| section02_perfect_forwarding (5) | `forwarding_reference_universal`, `reference_collapsing_rules`, `std_forward_usage`, `variadic_pack_forwarding`, `forward_like_cpp23` |
| section03_copy_elision (4) | `rvo`, `nrvo`, `guaranteed_copy_elision_cpp17`, `never_move_return_of_local` |
| section04_smart_pointers (11) | `unique_ptr_exclusive`, `unique_ptr_custom_deleter`, `make_unique`, `shared_ptr_shared`, `shared_ptr_control_block`, `make_shared_single_allocation`, `weak_ptr_break_cycles`, `enable_shared_from_this`, `raw_pointer_as_borrow`, `constexpr_unique_ptr_cpp23`, `out_ptr_inout_ptr_cpp23` |
| section05_lifetime_details (4) | `temporary_object_lifetime`, `const_ref_extends_temporary`, `dangling_reference_pointer`, `iterator_dangling_after_move` |

---

## 3. 覆盖矩阵 L × F

| L | 主要 cpp | 评级 | 备注 |
|---|----------|------|------|
| L1 | `move_construction`, `copy_construction` | **STRONG** | Buffer 风格深拷贝/接管指针 + 计数打印 |
| L2 | `valid_but_unspecified_state`, `move_assignment` | **PASS** | 自移动 + 标准库 string/vector 规则 |
| L3 | `std_move_static_cast` | **STRONG** | cast 本质、const 退化、具名 && |
| L4 | `noexcept_move_and_vector_growth` | **STRONG** | 扩容移动/拷贝对照；ThrowingMember 拖累；Rule of 0 |
| L5 | `simplified_implicit_move_cpp23` | **PASS** | P2266 叙述 + pass_through；破坏例注释 |
| L6 | `rvo`, `nrvo`, `guaranteed_copy_elision_cpp17` | **PASS+** | Immovable 展示 C++17 保证 |
| L7 | `never_move_return_of_local` | **STRONG** | good/bad 对照；成员/参数需 move |
| L8 | `forwarding_reference_universal`, `std_forward_usage`, `reference_collapsing_rules`, `variadic_pack_forwarding` | **STRONG** | broken wrapper 实证 |
| L9 | `forward_like_cpp23` | **PASS** | feature-test 门控；deducing this 回调 |
| L10 | `unique_ptr_*`, `raw_pointer_as_borrow`, **+pImpl 增强** | **STRONG** | 本次补 pImpl 不完整类型 + out-of-line dtor |
| L11 | `constexpr_unique_ptr_cpp23`, `out_ptr_inout_ptr_cpp23` | **PASS** | 瞬态分配说明已补强；clang-cl C++23 路径 |
| L12 | `shared_ptr_*`, `make_shared_*`, `weak_ptr_*`, `enable_shared_from_this` | **STRONG** | 别名构造、循环+手动拆环、bad_weak_ptr |
| L13 | section05 四文件 | **PASS** | 不故意造 ASan 可崩路径；注释引导本地 ASan |

**FAIL:** 0　**WEAK→抬升:** pImpl 缺口、constexpr unique_ptr 专家段　**全量 PASS+ 目标:** 达成

---

## 4. 检索补细节（forwarding / RVO）

| 主题 | 标准/提案 | 写入位置 |
|------|-----------|----------|
| `std::forward` 条件 cast | [forward]、EMC++ 23/25 | `std_forward_usage` |
| 引用折叠 `T& && → T&` | [dcl.ref] | `reference_collapsing_rules` |
| C++17 prvalue 不要求可移动 | [class.copy.elision] / [dcl.init] | `rvo`, `guaranteed_copy_elision_cpp17` |
| F.48 pessimizing-move | Core Guidelines | `never_move_return_of_local` |
| P2266 隐式移动始终 xvalue | P2266R3 | `simplified_implicit_move_cpp23` |
| P2445 `forward_like` | C++23 | `forward_like_cpp23` |
| pImpl + incomplete `unique_ptr` | EMC++ 18；析构完整性 | `unique_ptr_exclusive`（本次增强） |
| P2273 constexpr unique_ptr | transient allocation | `constexpr_unique_ptr_cpp23`（本次增强） |

---

## 5. 本次改动

| 文件 | 变更 |
|------|------|
| `section04_smart_pointers/unique_ptr_exclusive.cpp` | 补 **pImpl Facade**：不完整类型 `Impl` + 类外析构/移动；派生→基虚析构断言保留 |
| `section04_smart_pointers/constexpr_unique_ptr_cpp23.cpp` | 补瞬态分配边界、feature-test/与 shared_ptr 对照、运行期 move 断言 |

**编译：** `clang-cl -clang:-std=c++23`（与 `windows-debug` 预设一致）通过。

---

## 6. 残留 / 非本 stage 范围

- 文档 Qt 隐式共享对照：以注释/对照表为主，不单开 topic（符合「不扩无关空壳」）。
- 真泄漏 ASan 演示：`weak_ptr_break_cycles` 用「建环→手动拆」避免污染进程；报告中要求读者本地开 ASan 对照。
- `forward_like` / constexpr memory：依赖 `__cpp_lib_*` 门控；旧工具链走降级路径。

---

## 7. 验收清单对照（文档 14.1）

| 验收项 | 覆盖 |
|--------|------|
| 支持移动的资源类 | ✅ `move_construction` / `move_assignment` |
| `std::move` 为什么不 move | ✅ `std_move_static_cast` |
| 返回值别 `std::move` | ✅ `never_move_return_of_local` |
| C++23 简化隐式移动 | ✅ `simplified_implicit_move_cpp23` |
| 无泄漏对象图 + weak | ✅ `weak_ptr_break_cycles` |
| make_shared 好在哪 | ✅ `make_shared_single_allocation` |

**Stage05 结论：无 FAIL；WEAK 已抬升；可进入 R2 收口。**
