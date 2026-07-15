# Fill All C++ Topic Examples — Design Spec

**Date:** 2026-07-15
**Branch:** `fill/cpp-all-topic-examples`
**Worktree:** `.worktree/fill-cpp-all-topic-examples`
**Status:** Approved for planning

## 1. Goal

Replace all **874** empty `run()` placeholders under `src/cpp/` with rich, progressive, **safely runnable** C++23 demos that map 1:1 to `ArchitectureDesign/CppStudy` documentation topics — beginner through expert — then compile, commit, and push by stage (no PR).

## 2. Context

| Item | Value |
|------|--------|
| Repo | `C:\MyFile\ArcForges\LearnCpp` |
| Doc source | `C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\` |
| Topic count | 874 placeholders + 1 already-real (`ffm/learn_ffm.cpp`, out of scope) |
| Registry | `learn::topic<"id", run>` in each TU; `main` dispatches |
| Standard | C++23 (`CMAKE_CXX_STANDARD 23`, `/std:c++latest`) |
| Local toolchain | VS2026 Dev Prompt + `vcvars64.bat` path user provided |
| Empty pattern | `(void)argc; (void)argv; return 0;` body |

### 2.1 Doc ↔ code mapping

Each stage directory has `README.md` pointing at the Chinese CppStudy md. Each `.cpp` header lists:

- `Doc`, `Stage`, `Section`, `Item`, `Topic id`

Topic id form: `part2/stage01/section01/<item_slug>` (path-aligned).

| Stage directory | CppStudy document | ~Topics |
|-----------------|-------------------|---------|
| `part2_stage01_syntax_and_translation_model` | 第2部分-阶段1-语法基础与翻译模型.md | 19 |
| `part2_stage02_type_system_value_category_deduction` | 第2部分-阶段2-类型系统-值类别与类型推导.md | 53 |
| `part2_stage03_functions_overloading_lambdas` | 第2部分-阶段3-函数-重载-lambda.md | 35 |
| `part2_stage04_classes_and_object_model_basics` | 第2部分-阶段4-类与对象模型基础.md | 47 |
| `part2_stage05_copy_move_smart_pointers` | 第2部分-阶段5-拷贝与移动语义-智能指针.md | 32 |
| `part2_stage06_templates_and_generics` | 第2部分-阶段6-模板与泛型编程.md | 40 |
| `part2_stage07_compile_time_programming` | 第2部分-阶段7-编译期编程.md | 21 |
| `part2_stage08_control_flow_and_modern_syntax` | 第2部分-阶段8-控制流与现代语法增强.md | 20 |
| `part2_stage09_exceptions_error_handling_ub` | 第2部分-阶段9-异常-错误处理与UB纪律.md | 45 |
| `part2_stage10_stl_deep_dive` | 第2部分-阶段10-STL深潜.md | 132 |
| `part2_stage11_concurrency_and_threading` | 第2部分-阶段11-并发与多线程.md | 35 |
| `part2_stage12_coroutines` | 第2部分-阶段12-协程.md | 32 |
| `part2_stage14_keywords_and_features_index` | 第2部分-阶段14-特性与关键字全表.md | 18 |
| `part2_stage15_classic_idioms` | 第2部分-阶段15-C++经典惯用法.md | 22 |
| `part3_standard_library_systematized` | 第3部分-标准库系统化.md | 86 |
| `part4_engineering_system` | 第4部分-工程系统.md | 52 |
| `part6_branch_a_object_model` | 第6部分-支线A-对象模型.md | 16 |
| `part6_branch_b_lifetime_and_ownership` | 第6部分-支线B-生命周期与所有权.md | 29 |
| `part6_branch_c_memory_management` | 第6部分-支线C-内存管理.md | 29 |
| `part6_branch_d_name_lookup_overload_resolution` | 第6部分-支线D-名字查找ADL与重载决议.md | 23 |
| `part6_branch_e_templates_compile_time_system` | (roadmap embedded; no separate md) | 26 |
| `part6_branch_f_memory_model_and_concurrency` | 第6部分-支线F-内存模型与并发.md | 21 |
| `part6_branch_g_abi_compile_link_ub_performance` | 第6部分-支线G-ABI编译链接UB性能.md | 41 |

**Out of scope for this fill:** stage13 Modules body (already skipped by scaffold); `src/cpp/ffm/`; Kotlin/Java/Android/Spring trees.

## 3. Decisions (locked)

| Decision | Choice |
|----------|--------|
| Content density | Progressive multi-demo per file: beginner → intermediate → expert (2–5 segments) |
| Full Debug run-all | Every topic must exit 0; no intentional crash/UB execution |
| Missing C++23 features | `#if __cpp_*` / `__cpp_lib_*` (+ `__has_include`); skip message + safe fallback |
| Delivery | Per-stage: fill → build → smoke → `commit` + `push` |
| Execution architecture | Stage pipeline + parallel subagents per section |
| PR | None for now |

## 4. Architecture

### 4.1 Unchanged infrastructure

- Keep `include/learn/topic_registry.hpp` and `src/cpp/main.cpp` as-is unless a blocking defect appears.
- Keep CMake `GLOB_RECURSE` of `src/cpp/**/*.cpp` (excluding `ffm/`).
- Do not introduce a unit-test framework; demos are bare `int run(int, char**)`.

### 4.2 Per-file shape

```cpp
// LearnCpp topic example
// Doc      : <mapped doc stem>
// Stage    : <stage dir>
// Section  : <section dir>
// Item     : <item slug>
// Topic id : <registry id>
//
// Covers: <one-line knowledge summary from CppStudy>

#include "learn/topic_registry.hpp"
// standard headers as needed

namespace {

void demo_basics() {
    // entry-level runnable facts
}

void demo_intermediate() {
    // common modern usage / pitfalls (safe)
}

void demo_expert() {
    // deeper boundaries, idioms, feature-test gated C++20/23
    // NEVER invoke real UB / data race / null deref
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"<topic id>", run>;

}  // namespace
```

**Rules:**

1. Preserve existing topic id string exactly (registry contract).
2. Prefer `assert` for invariants that must hold; optional `std::cout`/`std::println` for teaching output.
3. Catch exceptions inside the topic if demonstrating exception paths; still return 0 after successful demo.
4. For UB-themed topics: show **safe** patterns and **comment** the forbidden pattern; do not execute UB.
5. For concurrency topics: join threads, avoid data races; TSan-clean under normal paths.
6. Feature gates: use language macros freely; for library macros `#include <version>` first.
7. No third-party deps unless already in the CMake project (none for topics today).
8. No comments except existing header style + brief Covers line + necessary feature-skip notes (project style: minimal comments; teaching demos may keep short section labels).

### 4.3 Source priority (research order per topic)

1. Matching section in CppStudy md (primary curriculum).
2. cppreference.com language/library pages + feature-test macros.
3. ISO draft / WG21 proposals when design rationale is required.
4. C++ Core Guidelines (Stroustrup/Sutter).
5. High-quality open reference implementations and patterns: libc++, libstdc++, MSVC STL, range-v3 idioms, Lewis Baker coroutine notes, well-known CppCon “Back to Basics” patterns — **adapted**, not copy-pasted under incompatible licenses without care. Prefer original minimal demos derived from public documentation.
6. learncpp.com / hackingcpp.com for beginner framing only.

### 4.4 Stage pipeline (repeat for each stage)

```text
1. Inventory: list all .cpp under stage; map to doc headings
2. Research pack: extract per-item bullet list of must-cover points
3. Parallel fill: subagents own 1+ sections (no overlapping files)
4. Build: vcvars64 → cmake preset build learn_cpp
5. Smoke: run each topic id in the stage; all exit 0
6. Review gate: spot-check ≥1 file per section
7. git commit (stage message) + git push -u origin HEAD
8. Next stage
```

Final gate after all stages:

- Full Debug no-arg run of `learn_cpp` (all topics) exits successfully.
- Spot-check counts: 874 non-empty `run()` bodies (no bare placeholder body).

### 4.5 Waves

| Wave | Stages | Approx topics |
|------|--------|---------------|
| W1 | stage01–05 | ~186 |
| W2 | stage06–09 | ~126 |
| W3 | stage10 STL | ~132 |
| W4 | stage11–12, 14–15 | ~107 |
| W5 | part3, part4 | ~138 |
| W6 | part6 A–G | ~185 |

Within a wave, stages still commit separately when build gates pass.

### 4.6 Build / verify commands (Windows)

```bat
%comspec% /k "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
```

Then (non-interactive equivalent in automation):

```pwsh
# From worktree root after vcvars environment is loaded:
cmake --preset windows-debug   # or project’s existing preset name
cmake --build --preset windows-debug
# Smoke one topic:
.\build\windows-debug\bin\learn_cpp.exe part2/stage01/section01/main_and_program_structure
# Stage batch: loop topic ids for that stage
```

Exact preset names must be taken from `CMakePresets.json` at implementation time.

## 5. Quality bar

### 5.1 Must

- Compiles as part of single `learn_cpp` target on the local VS18 toolchain.
- Every filled topic returns 0 on success path.
- Progressive demos present (not a single trivial line unless the topic is genuinely tiny, e.g. a pure keyword index row — still at least one non-trivial assertion).
- Doc mapping comment accurate.
- No secrets, no network required at runtime.

### 5.2 Should

- Cover the main acceptance bullets from the matching CppStudy section.
- Use modern C++ (prefer C++17/20/23 idioms over pre-C++11 style when teaching the modern path).
- Keep each file readable (~50–250 lines typical; expert sections may be longer when justified).

### 5.3 Must not

- Trigger real UB, intentional segfault, infinite hang, or unjoined threads.
- Change topic ids or delete topics.
- Add gtest/Catch2/etc. as a requirement to “pass”.
- Open a PR in this effort.
- Fill Kotlin/Java/Android/Spring placeholders.

## 6. Risks and mitigations

| Risk | Mitigation |
|------|------------|
| 874 files too large for one session | Stage pipeline + parallel subagents; resume from last committed stage |
| C++23 partial library support | Feature-test branches; document skip |
| Full run-all slow | Stage smoke first; full run only at end (and optionally after each wave) |
| Subagent quality drift | Shared file template + per-stage review gate |
| License contamination | Prefer original minimal demos; cite public docs, don’t dump large third-party sources |
| Compile break mid-stage | Fix before commit; never push a red build |

## 7. Success criteria

1. 874 topic `.cpp` files no longer match the empty placeholder body pattern.
2. Windows build of `learn_cpp` succeeds under user-specified `vcvars64` environment.
3. Debug full topic run completes with all topics returning success.
4. All stage commits pushed to `origin/fill/cpp-all-topic-examples`.
5. Spec + plan documents under `docs/superpowers/`.

## 8. Non-goals

- Implementing C++26 reflection/contracts/SIMD.
- Restoring full Modules stage13 tree.
- Performance tuning of the registry for 874 static initializers beyond current design.
- Writing companion markdown inside each topic directory (stage README already exists).

## 9. Next step

After user review of this spec file: invoke **writing-plans** to produce
`docs/superpowers/plans/2026-07-15-fill-cpp-topic-examples.md` with bite-sized tasks (inventory scripts, template, W1…W6 stage tasks, verify, push).
