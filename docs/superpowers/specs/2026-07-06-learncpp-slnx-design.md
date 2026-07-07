# LearnCpp.slnx — independent VS2026 / clang-cl / C++23preview build

- **Date:** 2026-07-06
- **Status:** Approved
- **Branch:** `worktree-add-learncpp-slnx`
- **Author:** sammiller (with Claude)

## Goal

Add a completely **CMake-independent** Visual Studio XML solution (`.slnx`) that builds the
`learn_cpp` executable with **VS2026's bundled clang-cl 22.1.3** at **`/std:c++23preview`**.
It must build locally with VS2026 and be verified by a new, parallel Windows CI job that uses
the same VS2026 environment. The solution is C++-only and has no relationship to CMake.

## Context (current state)

- Single `learn_cpp` executable built from **876 `.cpp`** files under `src/` plus one header
  `include/learn/topic_registry.hpp`. Topics self-register via inline variable templates at
  static-init time; `main()` dispatches.
- Build system today: CMake + Ninja, `CMAKE_CXX_STANDARD 23`, clang-cl on Windows. CMake
  currently passes `-std=c++23` (effective C++23, **not** `/std:c++latest`), so switching the
  independent build to `/std:c++23preview` (also C++23) carries low standard-level risk.
- Windows CI (`.github/workflows/windows-ci.yml`) already runs on `windows-2025-vs2026`,
  sets up VS2026 via `ilammy/msvc-dev-cmd@v1` (`vsversion: 18.0`), and uses the bundled
  clang-cl 22.x under `VC\Tools\Llvm\x64\bin` — but it builds via CMake only.
- `.vs/` is gitignored, so the `.slnx` cannot live there; it must live at the repo root.
- Local toolchain (verified): VS2026 Community v18.7 at
  `C:\Program Files\Microsoft Visual Studio\18\Community`; clang-cl 22.1.3 at
  `VC\Tools\Llvm\x64\bin`; MSBuild 18.7; ClangCl MSBuild toolset installed at
  `...\MSBuild\Microsoft\VC\v180\Platforms\x64\PlatformToolsets\ClangCL`.

## Confirmed decisions

1. **Solution file name:** `LearnCpp.slnx` (repo root).
2. **CI structure:** new **parallel** `slnx-msbuild` job; the existing CMake job is kept untouched.
3. **No `-Werror`** on the slnx build — keeps the independent build robust to flag-set
   differences. The CMake CI job continues to gate `-Werror` separately.
4. **Generated explicit project items + `.vcxproj.filters`.** VS2026's C++ project
   system does not reliably bind wildcard-expanded items to `.vcxproj.filters`, so
   `scripts/generate-vs-filters.ps1` writes every `src/` and `include/` file as an
   explicit project item and emits matching filters metadata for Solution Explorer.

## Artifacts (all new)

| Path | Purpose |
|---|---|
| `LearnCpp.slnx` | Minimal XML solution at repo root; references `vs/learn_cpp.vcxproj`; Debug + Release, x64. |
| `vs/learn_cpp.vcxproj` | ClangCl toolset, C++23preview, generated explicit source/header items, self-contained. |
| `.github/workflows/windows-ci.yml` | **Add** a new `slnx-msbuild` job (existing `build` job unchanged). |

No `Directory.build.props`, no `.user` files. A generated `.vcxproj.filters` file
uses the same Include strings as the explicit project items, which enables the IDE
folder tree.

## `.slnx` structure

```xml
<Solution Version="1.0" Description="LearnCpp — VS2026 clang-cl C++23preview (independent of CMake)">
  <Configurations>
    <BuildType Name="Debug" />
    <BuildType Name="Release" />
    <Platform Name="x64" />
  </Configurations>
  <Project Path="vs/learn_cpp.vcxproj" DisplayName="learn_cpp" />
</Solution>
```

## `.vcxproj` settings

| Setting | Value | Rationale |
|---|---|---|
| `PlatformToolset` | `ClangCl` | VS2026 bundled clang-cl 22.1.3 (toolset at `...\VC\v180\Platforms\x64\PlatformToolsets\ClangCL`). |
| `LanguageStandard` | `stdcpp23` | Emits `/std:c++23preview` (C++23). In VS2026 (v180) the enum value is `stdcpp23`, which maps to the `/std:c++23preview` switch (verified in `cl_clangcl_extension.xml`); `stdcpp23preview` is **not** a valid value in v180. |
| `ConfigurationType` / `SubSystem` | `Application` / `Console` | Executable. |
| `WarningLevel` | `Level4` (`/W4`) | Matches CMake. |
| `ConformanceMode` | `true` (`/permissive-`) | Matches CMake. |
| `ExceptionHandling` | `Sync` (`/EHsc`) | Matches CMake. |
| `AdditionalOptions` | `-Wno-unused-command-line-argument -Wno-unused-parameter -Wno-microsoft-include %(AdditionalOptions)` | Matches CMake's clang-cl noise-taming. |
| `PreprocessorDefinitions` | `_DEBUG` (Debug) / `NDEBUG` (Release) | So `main()` lists topics in Release (`NDEBUG`) and iterates in Debug — matches CMake's RelWithDebInfo. Without `NDEBUG`, `main()` took the debug branch and printed one line. |
| `AdditionalIncludeDirectories` | `$(RepoRoot)include` | Resolves `#include "learn/topic_registry.hpp"`. `RepoRoot = $([System.IO.Path]::GetFullPath('$(MSBuildThisFileDirectory)..\'))`, so paths work for direct vcxproj builds too (not just via the slnx). |
| `PrecompiledHeader` | `NotUsing` | Project uses none. |
| `TreatWarningAsError` | `false` | Decision #3 — independent build stays robust. |
| Sources | Generated explicit `<ClCompile Include="..\src\...\file.cpp">` and `<ClInclude Include="..\include\...\file.hpp" />` items. | VS C++ Solution Explorer expects project items to be explicit; re-run `scripts/generate-vs-filters.ps1` after add/remove. |
| `ObjectFileName` | Per-source generated `$(IntDir)src\...\file.obj`. | Explicit items do not populate `%(RecursiveDir)`, and duplicate basenames across `part*/section*` dirs need distinct `.obj` paths. |
| Parallelism | `UseMultiToolTask=true`, `MultiProcCL=true`, `MultiProcMaxCount=$([System.Environment]::ProcessorCount)`, `UseMsbuildResourceManager=true` + `MsbuildProcessCounter` scheduler | clang-cl has no `/MP`; MSBuild's MultiToolTask parallelizes the 876 TUs across cores. `MultiProcCL` is set **directly** (not via `UseMultiToolTask` alone) to work around a v180 target-ordering gap where the ClangCl fixup target reads `$(MultiProcCL)` before `FixupCLCompileOptions` sets it. Build with `-m`. |
| `OutDir` | `$(RepoRoot)build\vs\$(Platform)\$(Configuration)\` | Keeps output out of source; `build/` is gitignored. |
| `TargetName` | `learn_cpp` | Produces `build/vs/x64/Release/learn_cpp.exe`. |
| `IntDir` | `$(RepoRoot)build\vs\$(Platform)\$(Configuration)\obj\` | Keep `.obj` out of the source tree. |
| Configs | Debug (`MultiThreadedDebugDLL`, `/Od`, `/Z7`) + Release (`MultiThreadedDLL`, `/O2`), x64 only | Standard. |

All 876 `.cpp` (including `src\main.cpp` and `src\learn\topic_registry.cpp`) are compiled and
linked into the single exe, so every topic's inline `learn::topic<...>` specialization is
ODR-used and its static-init registers the topic — matching the CMake build's behavior.

## CI: new `slnx-msbuild` job in `windows-ci.yml`

- `runs-on: windows-2025-vs2026`, `timeout-minutes: 60`, `shell: pwsh`.
- `actions/checkout@v7`.
- `ilammy/msvc-dev-cmd@v1` with `arch: x64`, `vsversion: 18.0` (sets MSVC `INCLUDE`/`LIB`/`PATH` —
  identical to the existing CMake job).
- Locate VS2026 bundled clang-cl via `vswhere -latest -version "[18.0,19.0)"` →
  `VC\Tools\Llvm\x64\bin` → prepend to `GITHUB_PATH` (same logic as the existing job) so the
  ClangCl MSBuild toolset resolves `clang-cl.exe`.
- Verify: `MSBuild -version`, `clang-cl --version`.
- Build: `MSBuild LearnCpp.slnx -p:Configuration=Release -p:Platform=x64 -m`.
- Smoke-run: `build/vs/x64/Release/learn_cpp.exe` → expect **≥800 topic lines** (same threshold
  as the CMake job). Fail the job if fewer.
- **No sccache** — keeps the slnx build simple and CMake-independent.

The new job runs in parallel with the existing `build` (CMake) job and fails independently.

## Local verification (before push)

1. From a vcvars64 prompt (`scripts\dev-shell.cmd`), run
   `MSBuild LearnCpp.slnx -p:Configuration=Release -p:Platform=x64 -m` (~62s, parallel).
2. Confirm `build\vs\x64\Release\learn_cpp.exe` is produced.
3. Smoke-run → ≥800 topics; also build Debug to confirm both configs link and run.
4. Open `LearnCpp.slnx` in the VS2026 IDE to confirm it loads and builds.

## Independence from CMake (explicit)

The slnx/vcxproj compiles the generated explicit list of `src/**/*.cpp` +
`include/**/*.hpp` files directly via clang-cl. It reads **no** `CMakeLists.txt`,
`CMakePresets.json`, or `compile_commands.json`, and writes to `build/vs/`
(separate from CMake's `build/<preset>/`). After adding/removing a topic file, re-run
`scripts/generate-vs-filters.ps1`.

## Tradeoffs / known limitations

- **Build speed:** clang-cl has no `/MP`, so parallelism comes from MSBuild's
  `UseMultiToolTask` (MultiToolTask). Verified locally: **~62s** for 876 TUs on 18 cores
  (vs ~5–6 min sequential). CI uses plain `-m` (auto-sizes nodes to the runner);
  `MultiProcMaxCount=ProcessorCount` adapts the parallel width to the machine.
- **No `-Werror`:** the slnx build does not treat warnings as errors (CMake CI still does).
- **No sccache:** simpler, but cold CI builds are slower.
- **Generated source list:** new files require re-running
  `scripts/generate-vs-filters.ps1` so the project item list and filters stay in sync.

## Out of scope (YAGNI)

- `Directory.build.props` — single project; inline everything in the vcxproj.
- sccache integration with MSBuild.
- Linux/macOS slnx equivalents — slnx is VS-specific; other platforms keep CMake.

## Open questions

None — all design questions resolved during brainstorming.
