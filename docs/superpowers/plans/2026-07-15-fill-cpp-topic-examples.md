# Fill All C++ Topic Examples Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Fill all 874 empty `src/cpp/**/*.cpp` topic `run()` bodies with progressive beginner→expert demos mapped to CppStudy docs, compile on Windows, commit+push per stage (no PR).

**Architecture:** Keep `learn::topic<"id", run>` registry. Each file gains `demo_basics` / `demo_intermediate` / `demo_expert` (2–5 segments), always `return 0`. Stage pipeline: fill → `cmake --preset windows-debug` build → smoke topic ids → commit → push. Worktree: `.worktree/fill-cpp-all-topic-examples`, branch `fill/cpp-all-topic-examples`.

**Tech Stack:** C++23, CMake 3.28+, Ninja, clang-cl via VS18 `vcvars64.bat`, no third-party topic deps.

## Global Constraints

- Content density: progressive multi-demo per file (beginner → intermediate → expert, 2–5 segments).
- Full Debug run-all: every topic must exit 0; never execute real UB / intentional crash / unjoined threads.
- Missing C++23 features: `#if __cpp_*` / `__cpp_lib_*` (+ `#include <version>` for lib macros); skip message + safe fallback.
- Preserve existing topic id strings exactly.
- Do not add gtest; bare `int run(int, char**)`.
- Out of scope: `ffm/`, stage13 Modules body, Kotlin/Java/Android/Spring.
- Delivery: per-stage commit + push; no PR.
- Work only in worktree path: `C:\MyFile\ArcForges\LearnCpp\.worktree\fill-cpp-all-topic-examples`
- Doc root: `C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\`
- Build: load vcvars then `cmake --preset windows-debug` && `cmake --build --preset windows-debug`
- Binary: `build\windows-debug\bin\learn_cpp.exe <topic-id>`

### Canonical file shape (every topic)

```cpp
// LearnCpp topic example
// Doc      : <doc stem>
// Stage    : <stage>
// Section  : <section>
// Item     : <item>
// Topic id : <id>
//
// Covers: <one line>

#include "learn/topic_registry.hpp"
// needed headers

namespace {

void demo_basics() { /* ... */ }
void demo_intermediate() { /* ... */ }
void demo_expert() { /* ... */ }

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"<id>", run>;

}  // namespace
```

### Stage smoke helper (PowerShell)

```pwsh
$exe = "build\windows-debug\bin\learn_cpp.exe"
Get-ChildItem -Recurse -Filter *.cpp src\cpp\part2_stage01_* | ForEach-Object {
  # extract topic id from file content: learn::topic<"ID"
}
# Or run known ids:
& $exe part2/stage01/section01/main_and_program_structure
# expect exit 0
```

---

### Task 1: Stage01 — syntax and translation model (19 topics)

**Files:**
- Modify: all under `src/cpp/part2_stage01_syntax_and_translation_model/**/*.cpp` (19 files)
- Doc: `CppStudy/第2部分-阶段1-语法基础与翻译模型.md`

**Interfaces:**
- Consumes: `learn::topic`, existing topic ids
- Produces: filled run() for stage01; all exit 0

- [ ] **Step 1: Fill section01 (6 files)** — main, comments, iostream, builtin types, namespaces, inline_namespace
- [ ] **Step 2: Fill section02 (6 files)** — include text, guards, pragma once, elifdef/elifndef/warning C++23
- [ ] **Step 3: Fill section03 (7 files)** — TU model, preprocess/compile/link, declaration vs definition, ODR, header/source, undefined ref diagnosis, per-cpp independence
- [ ] **Step 4: Build + smoke all stage01 topic ids**
- [ ] **Step 5: Commit + push**

```bash
git add src/cpp/part2_stage01_syntax_and_translation_model
git commit -m "feat(cpp): fill stage01 syntax and translation model examples"
git push -u origin fill/cpp-all-topic-examples
```

---

### Task 2: Stage02 — type system (53 topics)

**Files:** `src/cpp/part2_stage02_type_system_value_category_deduction/**/*.cpp`
**Doc:** `第2部分-阶段2-类型系统-值类别与类型推导.md`

- [ ] Fill all section folders with progressive demos (value categories, init, cv, auto/decltype, casts, etc.)
- [ ] Build + smoke stage02 topic ids
- [ ] Commit + push: `feat(cpp): fill stage02 type system examples`

---

### Task 3: Stage03 — functions / overload / lambda (35 topics)

**Files:** `src/cpp/part2_stage03_functions_overloading_lambdas/**/*.cpp`
**Doc:** `第2部分-阶段3-函数-重载-lambda.md`

- [ ] Fill all; include default args, overload, ADL intro, lambda capture, generic lambda, C++23 static call operator where gated
- [ ] Build + smoke + commit + push

---

### Task 4: Stage04 — classes and object model basics (47 topics)

**Files:** `src/cpp/part2_stage04_classes_and_object_model_basics/**/*.cpp`
**Doc:** `第2部分-阶段4-类与对象模型基础.md`

- [ ] Fill all; special members overview, access, inheritance intro, virtual, deducing this gated
- [ ] Build + smoke + commit + push

---

### Task 5: Stage05 — copy/move/smart pointers (32 topics)

**Files:** `src/cpp/part2_stage05_copy_move_smart_pointers/**/*.cpp`
**Doc:** `第2部分-阶段5-拷贝与移动语义-智能指针.md`

- [ ] Fill all; Rule of 0/3/5, move, unique/shared/weak_ptr, enable_shared_from_this
- [ ] Build + smoke + commit + push

---

### Task 6: Stage06 — templates (40 topics)

**Files:** `src/cpp/part2_stage06_templates_and_generics/**/*.cpp`
**Doc:** `第2部分-阶段6-模板与泛型编程.md`

- [ ] Fill all; function/class templates, specialization, SFINAE intro, concepts
- [ ] Build + smoke + commit + push

---

### Task 7: Stage07 — compile-time programming (21 topics)

**Files:** `src/cpp/part2_stage07_compile_time_programming/**/*.cpp`
**Doc:** `第2部分-阶段7-编译期编程.md`

- [ ] Fill all; constexpr/consteval/constinit, if consteval, metaprogramming basics
- [ ] Build + smoke + commit + push

---

### Task 8: Stage08 — control flow modern syntax (20 topics)

**Files:** `src/cpp/part2_stage08_control_flow_and_modern_syntax/**/*.cpp`
**Doc:** `第2部分-阶段8-控制流与现代语法增强.md`

- [ ] Fill all; if init, structured bindings, range-for, attributes, spaceship
- [ ] Build + smoke + commit + push

---

### Task 9: Stage09 — exceptions / expected / UB discipline (45 topics)

**Files:** `src/cpp/part2_stage09_exceptions_error_handling_ub/**/*.cpp`
**Doc:** `第2部分-阶段9-异常-错误处理与UB纪律.md`

- [ ] Fill all; **safe** UB education only (no real UB execution); expected gated
- [ ] Build + smoke + commit + push

---

### Task 10: Stage10 — STL deep dive (132 topics)

**Files:** `src/cpp/part2_stage10_stl_deep_dive/**/*.cpp`
**Doc:** `第2部分-阶段10-STL深潜.md`

- [ ] Fill by section (containers, iterators, algorithms, ranges C++23, utilities, …)
- [ ] Build + smoke + commit + push (may split into 2 commits if needed: containers half / ranges half)

---

### Task 11: Stage11 concurrency (35) + Stage12 coroutines (32)

**Files:** stage11 + stage12 trees
**Docs:** stage11/12 md

- [ ] Fill concurrency safely (join all threads); coroutines with generator gated
- [ ] Build + smoke + commit + push per stage

---

### Task 12: Stage14 keywords index (18) + Stage15 idioms (22)

**Files:** stage14 + stage15 trees

- [ ] Fill checklists with runnable demos where possible; idioms with full progressive examples
- [ ] Build + smoke + commit + push per stage

---

### Task 13: Part3 standard library systematized (86)

**Files:** `src/cpp/part3_standard_library_systematized/**/*.cpp`
**Doc:** `第3部分-标准库系统化.md`

- [ ] Fill all sections
- [ ] Build + smoke + commit + push

---

### Task 14: Part4 engineering system (52)

**Files:** `src/cpp/part4_engineering_system/**/*.cpp`
**Doc:** `第4部分-工程系统.md`

- [ ] Fill; `import std` section may be feature-gated / compile-safe demos only
- [ ] Build + smoke + commit + push

---

### Task 15: Part6 branches A–G (185)

**Files:** `part6_branch_{a..g}_*/**/*.cpp`
**Docs:** corresponding 支线 md (branch E from roadmap)

- [ ] Fill A→G in order; deep topics stay safe-runnable
- [ ] Build + smoke + commit + push per branch

---

### Task 16: Final full-run verification + push

- [ ] Count non-empty topics == 874 (exclude placeholder body regex)
- [ ] Full Debug: `learn_cpp.exe` no-args runs all topics successfully
- [ ] Final commit if any fixes; ensure branch pushed

```pwsh
$exe = "build\windows-debug\bin\learn_cpp.exe"
& $exe
# Debug: should traverse all topics exit 0
```

---

## Self-review notes

- Spec coverage: all stages in design table have tasks; safety/feature-test/push rules in Global Constraints.
- No TBD placeholders in task goals; content is research-driven at fill time from CppStudy + cppreference.
- Topic ids never renamed.
