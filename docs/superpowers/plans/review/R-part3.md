# R-part3 · 标准库系统化 交叉 Review

**Date:** 2026-07-15
**Doc:** `C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\第3部分-标准库系统化.md`
**Code:** `src/cpp/part3_standard_library_systematized/` (86 `.cpp`)
**Baseline toolchain:** VS2026 clang-cl + MSVC STL (`/std:c++latest`)

---

## 1. 文档概念清单 L（摘要）

| 文档锚点 | 关键概念 / 验收点 |
|---|---|
| 库1 文本 | string 字节语义+SSO; contains/resize_and_overwrite C++23; string_view 悬垂; 编码 char/char8/16/32; format/print; to/from_chars; regex; locale 陷阱 |
| 库2 容器算法 | 全表复杂度/失效; flat_map/set; algorithm/numeric; execution 策略; ranges/fold; 比较器/哈希/投影; 异构 erase |
| 库3 工具 | optional/expected/variant/any/tuple; visit/apply; function vs move_only_function; bind_front/back; invoke_r; forward_like; structured bindings |
| 库4 时间数值 | chrono 时钟/日历/时区; random 引擎+分布; numbers; bit/bit_cast/byteswap; stdfloat/numeric_limits |
| 库5 IO | iostream 状态/manip; fstream/noreplace; spanstream; filesystem; print vs iostream; 序列化/mmap 边界说明 |
| 库6 内存 | 智能指针+constexpr unique_ptr; allocator/allocate_at_least; pmr; out_ptr; launder; start_lifetime_as |
| 库7 诊断 | stacktrace; unreachable; source_location; system_error |
| headers map | 诊断/类型/工具/内存/数值/文本/容器/迭代器算法/IO/时间/并发/协程/C 兼容; stdatomic.h vs stdbit C++26 |

---

## 2. 覆盖矩阵（Doc → Cpp）

| 文档块 | 覆盖 | 主要 topic | 等级 |
|---|---|---|---|
| 1.1 string / SSO / contains / resize_and_overwrite | covered | `std_string`, `std_string_contains_cpp23`, `std_string_resize_and_overwrite_cpp23` | PASS→STRONG |
| 1.2 string_view / contains / range ctor | covered | `std_string_view`, `string_view_contains_cpp23`, `string_view_range_constructor_cpp23` | PASS |
| 1.3 编码 / UTF 前缀 / 源文件 UTF-8 | covered | `byte_versus_char`, `char_encoding_utf_prefixes`, `utf8_source_file_cpp23` | PASS |
| 1.4 format / print / ranges format | covered | `std_format_*`, `std_print_println_cpp23` | PASS |
| 1.5 charconv / constexpr | covered | `to_chars_from_chars`, `constexpr_to_chars_from_chars_cpp23` | PASS |
| 1.6 regex / locale | covered | `std_regex`, `string_compare_and_locale_pitfalls` | PASS |
| 2.1 容器全表+复杂度 | covered | `sequence_associative_unordered_adapter_overview` | PASS |
| 2.2 flat_* | covered | `flat_containers_cpp23` | PASS |
| 2.3 algorithm/numeric/execution | covered | `algorithm_numeric_full_view`, `ranges_*` | PASS |
| 2.4 比较器/投影/异构 erase | covered | `custom_comparator_hash_projection`, `heterogeneous_erase_extract_cpp23` | PASS |
| mdspan（表 2.1） | covered (map 探针) | `section08/containers_headers` | PASS (地图级; 深讲在 stage10) |
| 3.x 工具类型 | covered | section03 全部 10 文件 | PASS |
| 4.x chrono/random/bit/numbers | covered | section04 全部 10 文件 | PASS |
| 5.x IO/filesystem | covered | section05 全部 12 文件 | PASS |
| 6.x 内存 | covered | section06 全部 12 文件 | PASS |
| 7.x 诊断 | covered | section07 全部 4 文件 | PASS |
| headers map 扫表 | covered | section08 全部 14 文件 | PASS |

**Doc → Cpp 缺口:** 无「完全无挂靠」的主线知识点。`mdspan` 仅 headers-map 级探针（文档定位为表项收口，阶段 10 已深讲容器视图类主题，可接受）。

**Cpp → Doc:** 额外工程笔记 `memory_mapped_files_note` / `serialization_landscape_note` 与文档 5.4「无标准、了解即可」一致。

---

## 3. 质量抽检（Primer 级）

| 文件 | 抽检结论 |
|---|---|
| `std_string.cpp` | SSO/字节语义/断言完整 — STRONG |
| `filesystem_overview_cpp17.cpp` | 路径/操作可运行 — STRONG |
| `pmr_memory_resource_cpp17.cpp` | pmr 资源切换 — PASS |
| `string_view_contains_cpp23.cpp` | 原 WEAK → 已补 empty/case/UTF/悬垂纪律 — PASS |
| `std_print_println_cpp23.cpp` | 原 0 assert → format 断言 + ostream 重载 — PASS |
| `diagnostics_headers.cpp` | 原 `assert(true)` → 真实 exception/system_error 断言 — PASS |
| `stacktrace_cpp23.cpp` | 多层调用 + diagnose 路径 + feature 降级 — PASS |

---

## 4. 本轮已做修改

### 增强（existing）

- `section01/.../string_view_contains_cpp23.cpp` — 入门/进阶/专家层；empty needle；UTF 字节注意；find 等价
- `section01/.../std_string_contains_cpp23.cpp` — 变异 string；Qt 对照说明
- `section01/.../std_print_println_cpp23.cpp` — format 断言；FILE*/ostream；无粘性状态对照
- `section01/.../std_format_runtime_versus_compile_time_check_cpp23.cpp` — 运行期非法格式路径断言
- `section03/.../std_visit_apply.cpp` — 多 variant visit；get_if/holds_alternative
- `section04/.../bit_cast_cpp20.cpp` — constexpr；endian；别名 UB 对照
- `section04/.../numbers_constants.cpp` — 更多常量恒等式；`pi_v<T>`
- `section05/.../std_print_cpp23.cpp` — IO 层 print vs iostream；hex 粘性对照
- `section06/.../constexpr_unique_ptr_cpp23.cpp` — array unique_ptr；删除器路径
- `section07/.../source_location_cpp20.cpp` — 错误捕获默认参；wrong_capture 反模式
- `section07/.../stacktrace_cpp23.cpp` — diagnose 集成；更强帧/文本检查
- `section08/.../diagnostics_headers.cpp` — 去掉空断言；logic_error/system_error
- `section08/.../memory_headers.cpp` — weak_ptr；allocator_traits；aligned new 对齐断言
- `section08/.../time_headers.cpp` — steady vs system；日历字段断言
- `section08/.../stdatomic_h_cpp23.cpp` — fetch_add；C++26 头陷阱断言

### 新增

- （无）本部分文档主线均有既有 item 可挂靠；优先增强。

---

## 5. 编译

- 独立 smoke：`build/part34_review_smoke`，对上述改动 TU `clang-cl /std:c++latest /c` **全部通过**。
- 全量 `cmake --build --preset windows-debug` 因并行 worktree 占用出现 ninja recompaction Permission denied（环境问题，非源码错误）；CMake 已重新收集 **880** 个 TU（含全局新增）。

---

## 6. 残留 / 后续可选

| 项 | 说明 |
|---|---|
| mdspan 专文 | 若希望 part3 与 flat_* 对等，可新增 `mdspan_overview_cpp23.cpp`（非阻塞） |
| execution::par | `algorithm_numeric_full_view` 仅 seq 稳定路径；par 可按平台 feature 再开（易环境相关） |
| regex 性能 | 文档强调慢；示例保持正确性优先 |

**目标状态:** 无 FAIL；原 thin/WEAK 已抬到 PASS+。
