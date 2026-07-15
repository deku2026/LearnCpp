# C++ 文档 ↔ Topic 交叉 Review 计划（高标准补全）

**Branch / worktree:** `feat/fill-cpp-topic-examples`
`C:\MyFile\ArcForges\LearnCpp\.worktrees\fill-cpp-examples`

**Goal:** 对现有 **全部已有 stage/目录**（不含 stage13 / part7 / part8）做「文档概念全集 × cpp 覆盖」交叉 review；实时检索补缺；把薄弱/遗漏点补进对应 `.cpp`（优先增强现有文件；仅当文档有明确知识点且无任何对应 item 时，才新增 `.cpp`）。

**明确不做：** `part2_stage13_modules`、`part7_performance_engineering`、`part8_abi_ffi_interop`。

---

## 1. Review 铁律

1. **以文档为权威清单**：从 `CppStudy/*.md` 抽出步骤/小节/标注（🟢⭐🆕🔶⚠️）、代码块、验收点、UB 清单。
2. **以 cpp 为交付证据**：每个知识点必须在至少一个 topic 里有可运行演示或**明确教学型可编译探针**（工程类允许探测+结构化输出）。
3. **交叉方向双向**
   - Doc → Cpp：文档有、代码无/弱 → **必须补**
   - Cpp → Doc：代码有、文档仅点到 → 用检索加深示例，不删代码
4. **禁止**脚本批量灌同质正文；允许按文件手工增强。
5. **实时检索**：cppreference、相关提案 P 编号、Boost/标准库实践（按主题需要）。
6. **不故意 UB**；feature-test 降级；保持 `learn::topic` id 稳定。
7. **每 stage 产出**：`docs/superpowers/plans/review/R-<stage>.md`（覆盖表 + 缺口 + 已做修改）。

---

## 2. 映射表（本轮范围）

| Stage 目录 | 文档 |
|---|---|
| part2_stage01 … 12, 14, 15 | 第2部分-阶段N-*.md |
| part3_standard_library_systematized | 第3部分-标准库系统化.md |
| part4_engineering_system | 第4部分-工程系统.md |
| part6_branch_a … g | 支线 A–G 对应 md / 路线图相关节 |

---

## 3. 单 Stage 工作流（严格）

```text
A. 读文档：列「概念/语法/特性/陷阱/验收」清单 L（带文档锚点）
B. 列该 stage 全部 .cpp 与 topic id 清单 F
C. 建覆盖矩阵：L × F（covered / weak / missing）
D. 对 weak/missing：检索 → 增强对应 .cpp（或新增 item 仅当完全无挂靠点）
E. 抽读 3–5 个最关键文件做 Primer 级质量抽检（深度、边界、断言）
F. 写 R-<stage>.md；本 stage 相关文件编译通过
```

### 质量门槛（文件级）

| 等级 | 标准 |
|------|------|
| FAIL | 仍是空壳/仅 assert(true)/与 item 无关 |
| WEAK | 只有入门一层；缺文档验收点/C++23/UB 对照 |
| PASS | 入门+进阶+专家；断言；对上文档关键点 |
| STRONG | 另含检索补到的易漏点、提案/标准细节、工程对照 |

目标：**无 FAIL**；WEAK 全部抬到 PASS+。

---

## 4. 并行波次

| Wave | 单元 |
|------|------|
| R1 | stage01, 02, 03, 04 |
| R2 | stage05, 06, 07, 08 |
| R3 | stage09, 11, 12, 14, 15 |
| R4 | stage10（按 section 切 3 路） |
| R5 | part3（2 路）, part4 |
| R6 | part6 A–D, E–G |
| R7 | 全量编译 + 汇总缺口表 + commit/push（不开 PR） |

---

## 5. 汇总交付

- `docs/superpowers/plans/review/00-CROSS-REVIEW-SUMMARY.md`：全局缺口与补全统计
- 代码改动：仅增强/新增 topic 源文件
- `cmake --preset windows-debug` 通过
- push 分支，**不开 PR**

---

## 6. 状态

- [ ] R1–R6 review + 补全
  - [ ] R1 stage01–04
  - [x] **R2 stage05–08**（2026-07-15）→ `review/R-stage05.md` … `R-stage08.md`
  - [ ] R3–R6
- [ ] R7 编译与推送
