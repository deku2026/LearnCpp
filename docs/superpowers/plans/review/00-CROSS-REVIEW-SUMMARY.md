# 全局交叉 Review 汇总

**日期：** 2026-07-15
**分支：** `feat/fill-cpp-topic-examples`
**范围：** 全部已有 stage（**不含** stage13 Modules / part7 / part8）

---

## 1. 方法

对每个 stage：

1. 读 `CppStudy` 对应文档 → 概念/验收/UB/C++23 清单
2. 对照 `src/cpp/**` topic 覆盖（covered / weak / missing）
3. 实时检索补易漏点；**增强现有 cpp**（必要时少量新增 item）
4. 产出 `docs/superpowers/plans/review/R-*.md`

---

## 2. 分报告索引

| 报告 | 范围 |
|------|------|
| R-stage01 … R-stage12,14,15 | 第2部分语言主线 |
| R-stage10 | STL 深潜（132 files，本轮重点加厚） |
| R-part3 / R-part4 | 标准库系统化 / 工程系统 |
| R-part6 | 支线 A–G |

---

## 3. 本轮主要成果（摘要）

| 区域 | 要点 |
|------|------|
| stage01–02 | 宏二次求值、值类别探针、auto 全家福、list-init 不回退、空壳 assert 清除 |
| stage03–04 | thin lambda/C++23 加厚；构造期虚调用不派发可观察断言 |
| stage05–08 | pImpl 不完整类型；P2718/assume/结构化绑定 maybe_unused |
| stage09/11/12/14/15 | expected_or_else 修复；UB 安全对照加厚；协程 vs Asio 注释；惯用法「何时用」矩阵 **新增** 2 topic |
| stage10 | **39** 文件加厚；验收 9/9；invalidation / 异质查找 / ranges 陷阱写入报告 |
| part3–4 | 去掉 assert(true)；**新增** FetchContent / CMakePresets 对照 topic |
| part6 | B/C/D 全量或近全量加深；F 内存序；G ABI/UB 安全阀 |

**FAIL 目标：** 各 R-*.md 报告无 FAIL 空壳。

---

## 4. 明确不做

- part2_stage13_modules
- part7_performance_engineering
- part8_abi_ffi_interop

---

## 5. 可接受残留 / 二波候选

- stage10 部分 mdspan policy / fold 兄弟专家层仍可再抬
- 真 ASan 崩溃、真跨 TU SIOF、多核 false-sharing 性能：注释引导，不在 topic 内故意 UB
- 部分 C++23 API 依赖 feature-test 回退（MSVC STL 差异）
- thin&lt;60 行统计仍可能包含「短但完整」的探针文件；以 R-*.md 质量等级为准

---

## 6. 验证

- 各波次改动 TU：`clang-cl -std=c++23` 抽检/分批编译
- 全量：`cmake --preset windows-debug` + build（见最终 commit 前日志）
