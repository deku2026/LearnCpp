# R-stage01 · 语法基础与翻译模型 · 文档×cpp 交叉 Review

**Date:** 2026-07-15
**Worktree:** `C:\MyFile\ArcForges\LearnCpp\.worktrees\fill-cpp-examples`
**Doc:** `ArchitectureDesign/CppStudy/第2部分-阶段1-语法基础与翻译模型.md`
**Code:** `src/cpp/part2_stage01_syntax_and_translation_model/` (19 `.cpp`)

---

## 1. 文档清单 L（步骤 / 概念 / 验收 / UB / C++23）

| ID | 锚点 | 要点 |
|----|------|------|
| L0 | §0 | 工具：godbolt / Insights / 本地 `-E/-S/-c`、Sanitizer |
| L1 | §1 | `main` 签名、隐式 `return 0`、启动序列、argc/argv、`EXIT_*`、hosted/freestanding |
| L2 | §2 | 工程四阶段；标准 9 翻译阶段；TU 定义；独立编译 |
| L2c23 | §2.3 | C++23：translation charset(P2314)；行拼接尾随空白修剪(P2223) |
| L3 | §3 | `#include` 文本粘贴；宏三类陷阱；条件编译；`__has_include` |
| L3c23 | §3.5 | `#elifdef` / `#elifndef` / `#warning` (P2334) |
| L4 | §4 | 声明 vs 定义；ODR 三条；`inline` 放宽 ODR；违规 NDR-UB；odr-use |
| L5 | §5 | 头/源分离；undefined/multiple definition；guard / `#pragma once`；linkage；`nm` |
| L6 | §6 | 基本类型目录；`sizeof` 序；`<cstdint>`；char 三型；字面值；`nullptr`；未初始化预告 |
| L6c23 | §6.5–6.6 | `<stdfloat>` 探测；`0uz`/`0z` |
| L7 | §7 | namespace；using 声明 vs 指令；匿名 NS=internal；别名；ADL 埋伏笔 |
| L8 | §8 | `cout/cin/cerr/clog`；`endl` vs `'\n'`；`ios_base::Init`；`print` 预告 |
| L9 | §9 | 注释；意图 vs 复述 |
| L10 | §10 | Qt 对照表（构建/moc/头源/nullptr/…） |
| L11 | §11 | UB/易错清单 10 条 |
| L12 | §12 | 验收 1–3 + 扩展练习 |

---

## 2. 文件清单 F（19）

| Section | Files |
|---------|-------|
| section01_language_basics | `main_and_program_structure`, `builtin_types_and_literals`, `namespaces`, `iostream_basic_io`, `comments`, `inline_namespace` |
| section02_preprocessor_and_includes | `include_text_substitution`, `include_guard`, `pragma_once`, `elifdef_cpp23`, `elifndef_cpp23`, `warning_directive_cpp23` |
| section03_translation_model | `preprocess_compile_assemble_link`, `translation_unit_concept`, `declaration_vs_definition`, `one_definition_rule_intro`, `header_source_separation`, `per_cpp_object_file_independence`, `undefined_reference_diagnosis` |

---

## 3. 覆盖矩阵摘要（Doc → Cpp）

| L | 映射 F | 初评 | 终评 |
|---|--------|------|------|
| L1 main | main_and_program_structure | WEAK（`EXIT_SUCCESS==0\|\|true` 空壳） | **PASS+** |
| L2 九阶段/四步 | preprocess_compile_assemble_link, translation_unit_concept, per_cpp… | PASS 缺 C++23 细节 | **PASS+** |
| L3 include+宏陷阱 | include_text_substitution | WEAK（仅 SQUARE 优先级） | **PASS+**（+二次求值/污染/__has_include） |
| L3c23 | elifdef/elifndef/warning_* | PASS；elifdef 有 `static_assert(true)` | **PASS+** |
| L4 ODR | one_definition_rule_intro, declaration_vs_definition | PASS 缺 odr-use 表述 | **PASS+** |
| L5 头源/链接/guard | header_source…, undefined_reference…, include_guard, pragma_once | pragma 空壳 assert | **PASS+** |
| L6 类型/字面值/nullptr | builtin_types_and_literals | PASS | PASS |
| L6c23 | 同上 feature-test | PASS | PASS |
| L7 namespace | namespaces, inline_namespace | PASS | PASS |
| L8 iostream | iostream_basic_io | PASS | PASS |
| L9 comments | comments | PASS | PASS |
| L10 Qt 对照 | 散落注释（main/iostream/namespaces） | 可接受（非独立 topic） | 可接受 |
| L11 UB 清单 | 各 topic 专家段 + uninitialized 预告 | PASS | PASS |
| L12 验收 | 对应 topic 可口述 + 命令清单 | PASS | PASS |

**Cpp → Doc：** `inline_namespace` 文档仅点到命名空间开放/嵌套，代码更深——保留，不删。

---

## 4. 本轮改动文件（6）

1. `section01_language_basics/main_and_program_structure.cpp`
   - 去掉 `static_assert(EXIT_SUCCESS == 0 || true)` 空壳；改为 `EXIT_FAILURE != EXIT_SUCCESS` 等有意义断言。
2. `section02_preprocessor_and_includes/include_text_substitution.cpp`
   - 补文档 §3.6 陷阱② `NAIVE_MAX` 副作用二次求值 + 函数式 `max_i` 对照；陷阱③ 无作用域污染说明；强化 model 字符串断言。
3. `section02_preprocessor_and_includes/pragma_once.cpp`
   - 去掉 `|| true`；断言库推荐 guard ≠ app once、Both 风格名。
4. `section02_preprocessor_and_includes/elifdef_cpp23.cpp`
   - `static_assert(true)` → 有意义的 `sizeof(int) >= 2` 分支活跃断言。
5. `section03_translation_model/preprocess_compile_assemble_link.cpp`
   - 阶段 2 写入 C++23 P2223 尾随空白修剪；强化 phase/artifact 断言与 `nm` 心智注释。
6. `section03_translation_model/one_definition_rule_intro.cpp`
   - 补 **odr-use** 教学释义、规则③ 字符串断言、inline static 票据再断言。

---

## 5. 检索补强要点

- [cppreference · Phases of translation](https://en.cppreference.com/cpp/language/translation_phases)：9 阶段分组；C++23 行拼接空白。
- [cppreference · Definitions and ODR](https://en.cppreference.com/cpp/language/definition)：三条 ODR；inline 链接语义；NDR。
- [cppreference · Preprocessor / conditional](https://en.cppreference.com/cpp/preprocessor/conditional)：`#elifdef`/`#elifndef`。
- 宏：参数多重展开副作用 → 函数/模板替代（与文档陷阱②一致）。

---

## 6. 抽检（PASS+）

| 文件 | 结果 |
|------|------|
| preprocess_compile_assemble_link | PASS+（九阶段 + C++23 + 命令/符号） |
| one_definition_rule_intro | PASS+（三条 ODR + inline + odr-use） |
| include_text_substitution | PASS+（三类宏陷阱 + include 模型） |
| undefined_reference_diagnosis | PASS（未改；诊断剧本完整） |
| main_and_program_structure | PASS+ |

**编译：** 上述 6 个改动文件 + 关联 obj 在 `windows-debug` / clang-cl C++23 下 **单独编译通过**。全量 `learn_cpp` 链接当前被 **stage09 `expected_or_else`** 无关错误阻断（不在本 stage 范围）。

---

## 7. 仍可接受的缺口

| 缺口 | 理由 |
|------|------|
| 真实多文件 `-c` + 链接演示 | 单 topic 单 TU 模型限制；`undefined_reference_diagnosis` / `header_source_separation` 用结构化剧本代替 |
| 故意 ODR 布局不一致的运行时 UB | 禁止故意 UB；专家段文字说明 NDR |
| `nm`/`c++filt` 运行期调用 | 平台工具依赖；命令写在注释中 |
| Qt moc 独立 topic | 文档对照表即可；非 C++ 语言本体 |
| freestanding 入口 | 文档「了解即可」 |

---

## 8. 结论

- **FAIL：0**
- **WEAK → PASS+：本轮已抬升（main/pragma/include 宏陷阱/ODR/九阶段）**
- stage01 文档主线验收点均有可运行或教学型探针覆盖。
