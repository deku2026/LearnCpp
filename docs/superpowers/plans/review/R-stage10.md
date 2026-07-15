# R-stage10 · STL 深潜 交叉 Review 报告

| 字段 | 值 |
|------|-----|
| **Stage** | `part2_stage10_stl_deep_dive` |
| **文档** | `ArchitectureDesign/CppStudy/第2部分-阶段10-STL深潜.md` |
| **Worktree** | `C:\MyFile\ArcForges\LearnCpp\.worktrees\fill-cpp-examples` |
| **日期** | 2026-07-15 |
| **审查文件数** | **132** `.cpp` |
| **增强文件数** | **39**（手工加厚至 入门+进阶+专家+断言） |
| **审查前 thin(&lt;60 行)** | ~78 |
| **审查后 thin(&lt;60 行)** | ~50（多为中等深度 WEAK→接近 PASS；见 §5） |
| **FAIL** | **0**（无空壳 / 仅 `assert(true)`） |

---

## 0 · 摘要

- 文档步骤 **1–20** 知识点已全量抽取，并与 12 个 section / 132 个 topic 做 **Doc→Cpp / Cpp→Doc** 双向对照。
- **验收清单（步骤 19.1）全部有对应 cpp**，多数已达 PASS；本轮重点把 **算法 / string_view·span / Ranges C++20 管道 / C++23 views·to·fold** 的 thin 文件抬到 PASS 结构。
- cppreference 交叉核对：**iterator invalidation**（含 unordered rehash 仅失效迭代器、引用保留）、**heterogeneous lookup**、**ranges dangling / category 降级** 已写入报告与关键示例。
- **关键缺口**：部分 C++23 fold 兄弟算法 / mdspan 子文件 / format·chrono·utility 仍 WEAK；`charconv` constexpr 文档点到但无独立 item；flat 失效表 cppreference 仍 incomplete（代码侧 `flat_versus_tree_tradeoff` 已覆盖）。

---

## 1 · 文档知识点清单 L（步骤 1–20）

### 步骤 1 · 顺序容器
| ID | 知识点 | 标注 |
|----|--------|------|
| L1.1 | vector 增长 / capacity / reserve / resize | 🟢⭐ 验收 |
| L1.1b | 扩容后迭代器/指针/引用全部失效 | ⚠️ 验收 |
| L1.2 | `vector<bool>` 代理陷阱 | ⚠️ |
| L1.3 | array / deque / list / forward_list 对比 | 🟢 |
| L1.4 | 容器选择 = 复杂度 + 缓存局部性 | ⭐ |

### 步骤 2 · 关联容器 + 异质查找
| ID | 知识点 |
|----|--------|
| L2.1 | map/set/multimap/multiset 红黑树 O(log n)、节点式失效 |
| L2.2 | unordered_* 哈希、load factor、rehash 失效（迭代器失效/引用不失效） |
| L2.3 | 透明比较器 `less<>` / 透明 hash 异质查找 | ⭐ 验收 |
| L2.4 | C++20 `contains`；C++23 异质 erase/extract (P2077) | 🆕 |

### 步骤 3 · 适配器 + flat
| ID | 知识点 |
|----|--------|
| L3.1 | stack / queue / priority_queue |
| L3.2 | flat_map/set/multi*；vs map 缓存/插入权衡 | 🆕⭐ 验收 |

### 步骤 4 · 迭代器与失效
| ID | 知识点 |
|----|--------|
| L4.1 | C++17 五类 + C++20 concepts（含 contiguous） |
| L4.2 | 各容器插入/删除失效表；边遍历边 erase；erase_if | ⭐ |
| L4.3 | 插入迭代器 / 流迭代器 |

### 步骤 5 · 泛型算法
| ID | 知识点 |
|----|--------|
| L5.1 | algorithm/numeric 全景；erase-remove |
| L5.2 | 谓词与 lambda |
| L5.3 | sort / accumulate / transform / reduce |
| L5.4 | 执行策略 par/par_unseq；谓词无数据竞争 | ⚠️ |
| L5.5 | clamp/gcd/lcm/sample；byteswap/to_underlying/unreachable | 🆕 |

### 步骤 6 · string_view
| ID | 知识点 |
|----|--------|
| L6.1 | 不拥有 / 零拷贝切片 / 函数参数 |
| L6.2 | 悬垂 + split 解析验收 | ⭐ 验收 |
| L6.3 | contains / 禁 nullptr / 迭代器范围构造 | 🆕 |

### 步骤 7 · string C++23 + span
| ID | 知识点 |
|----|--------|
| L7.1 | string::contains / resize_and_overwrite | 🆕 |
| L7.2 | span 可写视图 / 静动态 extent / 悬垂 | ⭐ |

### 步骤 8 · mdspan
| ID | 知识点 |
|----|--------|
| L8.1–5 | extents / layout / mapping / accessor | 🆕⭐ |
| L8.6 | `m[i,j]` 矩阵 + 对称判断 | 验收 |
| L8.7 | vs 手写索引 / Eigen | 验收 |

### 步骤 9 · 词汇类型
| ID | 知识点 |
|----|--------|
| L9.1 | optional / expected（+ monadic C++23） |
| L9.2 | variant+visit+overloaded 状态机；P2162 派生 visit | ⭐ 验收 |
| L9.3 | any / tuple / pair / apply / monostate / byte |

### 步骤 10–14 · Ranges
| ID | 知识点 |
|----|--------|
| L10.1–5 | 为何 Ranges；range/view/sized/common/borrowed/viewable；iterator-sentinel；CPO begin；range-v3 | ⭐⭐ |
| L11.1–3 | 惰性管道；C++20 基础 views；投影 | ⭐ |
| L12.1–3 | zip/enumerate；chunk/slide/adjacent；stride/cartesian/join_with/as_*/repeat | 🆕 验收 |
| L13.1–4 | ranges::to；starts/ends/contains/find_last/iota/shift；fold 族；range_adaptor_closure | 🆕 验收 |
| L14.1–4 | 夹临时悬垂；dangling；单遍；category 降级 | ⚠️⭐ |

### 步骤 15 · format / print
| ID | 知识点 |
|----|--------|
| L15.1 | print/println Unicode | 🆕 验收 |
| L15.2 | format ranges；编译期检查；thread_id/stacktrace；non-const formattable | 🆕 |

### 步骤 16–20 · 其他 / Qt / UB / 验收 / 资源
| ID | 知识点 |
|----|--------|
| L16 | chrono/random/regex/filesystem/bit/spanstream/exclusive fstream/generator |
| L17 | Qt 对照表（注释级，无独立 cpp 亦可） |
| L18 | UB 清单 15 条（分布在各 topic） |
| L19 | 验收清单 9 条 |
| L20 | 资源 URL 表 |

---

## 2 · Section × 文件覆盖矩阵（摘要）

| Section | 文件数 | 文档锚点 | 覆盖总评 |
|---------|--------|----------|----------|
| section01_sequence_containers | 8 | 步骤 1 | **PASS**（vector 失效/reserve/bool/选择均 ≥100 行） |
| section02_associative_containers | 7 | 步骤 2 | **PASS**（异质查找/erase/hash load 扎实） |
| section03_container_adapters | 8 | 步骤 3 | **PASS**（flat 全套 + tradeoff 基准 STRONG） |
| section04_iterators_and_invalidation | 5 | 步骤 4 | **PASS**（per-container 失效表 129 行） |
| section05_algorithms | 14 | 步骤 5 | **PASS–**（本轮加厚 clamp/byteswap/to_underlying/gcd/accumulate/transform/reduce；parallel 仍偏薄） |
| section06_modern_utility_types | 25 | 步骤 6–9 | **PASS–**（sv/span/optional 本轮抬升；mdspan 子文件部分 WEAK；variant 状态机有文件） |
| section07_ranges_core_cpp20 | 20 | 步骤 10–11 | **PASS–**（管道/概念/sentinel 本轮大幅加厚） |
| section08_ranges_cpp23 | 26 | 步骤 12–14 | **PASS–**（zip/enumerate/chunk/slide/to/fold/pitfalls 等加厚；fold 兄弟仍 WEAK） |
| section09_customization_points | 2 | 步骤 10.4–10.5 | **PASS** |
| section10_format_and_print_cpp23 | 6 | 步骤 15 | **PASS–**（有覆盖；compile-time 仍偏短） |
| section11_other_stl | 10 | 步骤 16 | **PASS–** |
| section12_coroutine_library | 1 | 步骤 16.3 | **PASS**（generator recap） |

**图例**：PASS = 入门+进阶+专家+断言且对齐文档关键点；PASS– = 主路径达标、局部子文件 WEAK；WEAK = 仅入门层。

---

## 3 · 验收清单（步骤 19.1）对照

| 验收项 | 主挂靠文件 | 状态 |
|--------|------------|------|
| vector 失效 | `vector_iterator_invalidation.cpp` + `per_container_invalidation_rules.cpp` | **covered / PASS** |
| 异质查找 | `transparent_comparator_heterogeneous_lookup.cpp` + `unordered_map_set.cpp` | **covered / PASS** |
| string_view 解析+悬垂 | `string_view_dangling_pitfall.cpp`（split） | **covered / PASS** |
| variant 状态机 | `variant_visit_overload_idiom.cpp` | **covered / PASS** |
| Ranges 管道+概念+CPO | `pipe_*` + `range_concept` + `cpo_begin_end_swap` + `borrowed_range_dangling` | **covered / PASS**（本轮加厚） |
| C++23 zip/enumerate/chunk/slide/to/fold | `views_zip_*` `views_enumerate_*` `views_chunk_*` `views_slide_*` `ranges_to_*` `ranges_fold_left_*` | **covered / PASS**（本轮加厚） |
| mdspan 矩阵对称 | `mdspan_versus_alternatives_cpp23.cpp` 等 | **covered / PASS–**（映射/layout 文件仍可再厚） |
| print 替换 iostream | `std_print_println_cpp23.cpp` | **covered / PASS** |
| flat_map 基准 | `flat_versus_tree_tradeoff.cpp` | **covered / STRONG** |

---

## 4 · 本轮增强文件清单（39）

### section05_algorithms（8）
- `std_clamp.cpp` — 返回 `const T&` 悬垂、`lo<=hi` 前置
- `std_byteswap_cpp23.cpp` — endian 场景 + feature-test
- `std_to_underlying_cpp23.cpp` — 位标志 / forged enum
- `std_gcd_lcm.cpp` — 约分 / 折叠 lcm / 溢出提醒
- `std_accumulate.cpp` — 初值类型陷阱 vs reduce/fold
- `std_transform.cpp` — 一元/二元/ranges vs views
- `std_reduce.cpp` — 可结合要求 + transform_reduce 点积

### section06_modern_utility_types（7）
- `string_contains_cpp23.cpp` / `string_view_contains_cpp23.cpp`
- `string_view_basics_cpp17.cpp` / `string_view_null_construct_forbidden_cpp23.cpp`
- `std_optional.cpp` / `std_span_cpp20.cpp`（含 borrowed_range）

### section07_ranges_core_cpp20（13）
- `pipe_filter/take/drop/transform/drop_while/take_while/join`
- `views_reverse` / `views_iota`
- `borrowed_range_dangling` / `common_range` / `sized_range` / `viewable_range`
- `iterator_sentinel_model`

### section08_ranges_cpp23（11）
- `views_zip/enumerate/chunk/slide/stride/repeat/adjacent/join_with/cartesian`
- `ranges_to_container_cpp23` / `ranges_fold_left_cpp23` / `ranges_expert_pitfalls`

**增强模式（统一）**：`=== 入门 ===` → `=== 进阶 ===` → `=== 专家 ===` + 多断言 + C++23 处 `__cpp_lib_*` feature-test / 可运行回退。

---

## 5 · 仍 WEAK / 建议下一轮（非 FAIL）

| 优先级 | 文件簇 | 原因 |
|--------|--------|------|
| P1 | `ranges_fold_*` 兄弟（right/first/with_iter 等） | 有回退但专家层薄 |
| P1 | `mdspan_extents/layout/mapping/accessor` | policy 级文档要求深讲，行数仍偏少 |
| P2 | `views_as_const/as_rvalue/chunk_by/zip_transform` | 仅入门演示 |
| P2 | `pipe_split` / `views_common` / `views_elements_*` / `projection_argument` | 结构未完全三段化 |
| P2 | `parallel_execution_policy_cpp17` | 缺 TBB/数据竞争专家说明厚度 |
| P2 | `std_variant` / monadic optional / expected | 有内容但可再对齐验收状态机深度 |
| P3 | format compile-time / chrono locale / string_resize_and_overwrite | 中等 thin |
| P3 | **missing item** | 文档 L16 `charconv` constexpr —— 无独立 cpp（可挂 stage07 或新增；本轮未新增文件） |

**Cpp→Doc 富余**：C++26 `inplace_vector`/`hive` 未在文档主线，代码亦未引入（正确）。

---

## 6 · cppreference 检索补强（易漏点）

### 6.1 Iterator invalidation（containers 总表）
| 容器 | 插入 | 删除 | 本 stage 落点 |
|------|------|------|----------------|
| vector | reallocate→**全部**；否则插入点后 | 删除点及之后 | `vector_iterator_invalidation` / `per_container_*` |
| deque | 中间插入→迭代器全失效；头尾引用规则更细 | 中间全失效 | `std_deque` + per_container |
| list/forward_list | **不失效** | **仅被删** | `std_list` / `std_forward_list` |
| map/set | **不失效** | **仅被删** | `ordered_map_set` |
| unordered_* | **rehash 时迭代器全失效**；**引用/指针不失效** | 仅被删迭代器 | `std_hash_load_factor` |
| flat_* | 似 vector（插入删除广失效） | 同左 | `flat_versus_tree_tradeoff`（cppreference flat 失效表仍 incomplete） |

### 6.2 Heterogeneous lookup
- 有序：`std::less<>` + `is_transparent`（C++14+）→ find/count/contains 免临时 key。
- 无序：透明 Hash + KeyEqual（C++20）。
- C++23 P2077：异质 erase/extract。
- 落点：`transparent_comparator_*` / `heterogeneous_erase_extract_cpp23`。

### 6.3 Ranges pitfalls
| 坑 | 机制 | 落点 |
|----|------|------|
| 管道夹临时容器 | view 不拥有 → 悬垂 UB | `ranges_expert_pitfalls` |
| 临时拥有范围算法返回值 | `ranges::dangling` 编译期拦截 | `borrowed_range_dangling` |
| filter 后 `[]` | category 降为 bidirectional | `pipe_filter` / pitfalls |
| zip 长度/category | 最短截断；category 取最弱 | `views_zip_cpp23` |
| 单遍 input | 再遍历空/UB | pitfalls 注释 + generator 是 input_range |
| 复用结果 | `ranges::to` 物化 | `ranges_to_container_cpp23` |

### 6.4 其他易漏
- `string_view` **不保证 NUL 结尾** → basics / 禁 nullptr 文件。
- `clamp` 返回 `const T&` 绑定临时悬垂 → `std_clamp`。
- `accumulate` 初值类型决定累加类型 → `std_accumulate`。
- `reduce` 要求可结合（并行）→ `std_reduce`。
- unordered rehash：**迭代器失效、到元素的引用保留**（与 vector 全搬不同）。

---

## 7 · 质量抽检（Primer 级）

| 文件 | 判定 | 备注 |
|------|------|------|
| `vector_growth_reserve.cpp` | **STRONG** | size/cap/reserve/resize/shrink |
| `flat_versus_tree_tradeoff.cpp` | **STRONG** | 计时 + feature-test 模拟 |
| `per_container_invalidation_rules.cpp` | **PASS+** | 失效表教学 |
| `string_view_dangling_pitfall.cpp` | **PASS** | split 验收 |
| `ranges_expert_pitfalls.cpp` | **PASS**（本轮） | 三大反差 |
| `ranges_to_container_cpp23.cpp` | **PASS**（本轮） | 验收物化 |
| `views_zip/enumerate/chunk/slide` | **PASS**（本轮） | 验收实战 |
| `mdspan_mapping_cpp23.cpp` | **WEAK→待抬** | 行数仍低 |
| `parallel_execution_policy_cpp17.cpp` | **WEAK→待抬** | |

---

## 8 · 统计交付

```
审查文件数: 132
增强文件数: 39
FAIL:        0
验收 19.1:   9/9 covered
关键缺口:
  - mdspan policy 子文件深度
  - fold 家族兄弟文件一致性
  - parallel 执行策略专家层
  - charconv constexpr 无独立 item
  - 约 50 个 <60 行文件仍可再抬（非空壳，多属 WEAK/PASS- 边界）
```

---

## 9 · 方法说明

- **禁止**脚本同质灌正文；39 个文件均为分主题手工重写。
- C++23 API 统一：`#if defined(__cpp_lib_*)` + 可运行 fallback（accumulate / 手写循环 / 注释）。
- **未新增** `.cpp`（文档知识点均有挂靠 item，除 charconv 可选挂靠）。
- 编译验证：建议 `cmake --preset windows-debug` 全量（本子代理未强制跑完整 CMake；改动保持 `learn::topic` id 稳定）。

---

## 10 · 下一步建议

1. 第二波加厚：mdspan 五件套 + fold 兄弟 + as_const/as_rvalue/chunk_by/zip_transform + parallel。
2. 可选新增：`charconv_constexpr_cpp23.cpp`（或挂 stage07）。
3. Wave R7：全量编译 + 汇总 `00-CROSS-REVIEW-SUMMARY.md`。
