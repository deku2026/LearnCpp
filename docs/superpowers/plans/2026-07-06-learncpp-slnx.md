# LearnCpp.slnx Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

> **STATUS: Implemented (2026-07-06).** The committed `vs/learn_cpp.vcxproj` is authoritative.
> During build verification the vcxproj was refined beyond the Task 1 Step 1 snippet below:
> `UseMultiToolTask` + `MultiProcCL=true` + `MultiProcMaxCount=ProcessorCount` + the
> `MsbuildProcessCounter` scheduler were added for parallel compilation (clang-cl has no `/MP`);
> `PreprocessorDefinitions` (`NDEBUG`/`_DEBUG`) were added; and source/output paths use
> `$(RepoRoot)` instead of `..\` / `$(SolutionDir)`. Update (2026-07-07): VS2026 still displayed
> wildcard-expanded files at the project root, so the final vcxproj now uses generated explicit
> project items plus matching `.vcxproj.filters` entries. Each source gets a generated
> `ObjectFileName=$(IntDir)src\...\file.obj` to avoid duplicate-basename collisions. The vcxproj
> snippet below is the **initial** version — see the spec and implementation commits for the
> final, verified content.

**Goal:** Add a CMake-independent Visual Studio XML solution (`LearnCpp.slnx`) that builds the `learn_cpp` executable with VS2026's bundled clang-cl at `/std:c++23preview`, verify it locally, and add a parallel Windows CI job that builds it with the same VS2026 environment.

**Architecture:** A hand-written `.vcxproj` under `vs/` uses the `ClangCl` MSBuild platform toolset (which auto-resolves VS2026's bundled `clang-cl.exe` via `VsInstallRoot`) and a generated explicit list of `src/**/*.cpp` + `include/**/*.hpp`, so it is fully independent of CMake while giving the VS C++ IDE stable project items for Solution Explorer filters. A minimal `.slnx` at the repo root references that vcxproj with Debug + Release / x64 configurations. A new `slnx-msbuild` job in `windows-ci.yml` mirrors the existing cmake job's VS2026 setup and builds the slnx with MSBuild, then smoke-runs the exe.

**Tech Stack:** MSBuild (VS2026 v18.7), `ClangCl` platform toolset, clang-cl 22.1.3 (`/std:c++23preview` via `LanguageStandard=stdcpp23`), `.slnx` XML solution format, GitHub Actions (`windows-2025-vs2026` runner, `ilammy/msvc-dev-cmd@v1`).

## Global Constraints

- **C++ standard:** `LanguageStandard=stdcpp23` (the VS2026 v180 enum value that emits `/std:c++23preview`). Do NOT use `stdcpp23preview` — it is not a valid value in v180.
- **Compiler:** `PlatformToolset=ClangCl` (VS2026 bundled clang-cl 22.1.3). No external LLVM.
- **Warning flags:** `/W4 /permissive- /EHsc` plus `-Wno-unused-command-line-argument -Wno-unused-parameter -Wno-microsoft-include`. **No `/WX`** (no `-Werror`) on the slnx build — the CMake CI job continues to gate `-Werror` separately.
- **Independence:** The vcxproj must read no `CMakeLists.txt`, `CMakePresets.json`, or `compile_commands.json`, and must write to `build/vs/` (separate from CMake's `build/<preset>/`).
- **Line endings:** `.gitattributes` declares `*.vcxproj text eol=crlf` and everything else `eol=lf`. The pre-commit `mixed-line-ending` hook (`--fix=lf`) must exclude `.vcxproj` so it does not fight `.gitattributes` and break CI's `pre-commit run --all-files`.
- **No `Directory.build.props`, no `.user` files** — minimal, self-contained project.
  `scripts/generate-vs-filters.ps1` writes explicit vcxproj source/header items and
  a matching `.vcxproj.filters` file so VS2026 can mirror CMake's folder tree in
  Solution Explorer.
- **Pre-commit:** Repo uses pre-commit (trailing-whitespace, end-of-file-fixer, mixed-line-ending, check-yaml, codespell, clang-format). clang-format only runs on `*.{c,h,cpp,hpp,...}` so it skips `.vcxproj`/`.slnx`. Run `pre-commit` on new files before committing to avoid the modify-restage dance.
- **Worktree:** Work happens in the worktree at `C:\MyFile\LearnCpp\.claude\worktrees\add-learncpp-slnx` on branch `worktree-add-learncpp-slnx`. The PowerShell/Bash working directory is already there — do not `cd`.
- **Local VS2026 path:** `C:\Program Files\Microsoft Visual Studio\18\Community`; vcvars64 at `VC\Auxiliary\Build\vcvars64.bat`; MSBuild 18.7.

## File Structure

| Path | Status | Responsibility |
|---|---|---|
| `vs/learn_cpp.vcxproj` | Create | MSBuild project: ClangCl toolset, C++23preview, generated explicit sources, Debug+Release/x64, output to `build/vs/`. Self-contained — the entire build definition. |
| `LearnCpp.slnx` | Create | Minimal XML solution at repo root; references `vs/learn_cpp.vcxproj`; declares Debug/Release + x64. Entry point for VS IDE and `MSBuild LearnCpp.slnx`. |
| `.pre-commit-config.yaml` | Modify | Add `vcxproj` to `mixed-line-ending` exclude so the hook respects `.gitattributes` (`*.vcxproj eol=crlf`) and CI's `pre-commit --all-files` stays green. |
| `.github/workflows/windows-ci.yml` | Modify | Add a new `slnx-msbuild` job (parallel to the existing `build` job). |
| `docs/superpowers/specs/2026-07-06-learncpp-slnx-design.md` | Exists | Approved design spec (already committed). |

---

### Task 1: Create the MSBuild project + solution, build Release locally, commit

**Files:**
- Create: `vs/learn_cpp.vcxproj`
- Create: `LearnCpp.slnx`
- Modify: `.pre-commit-config.yaml` (the `mixed-line-ending` hook's `exclude`)

**Interfaces:**
- Consumes: all `src/**/*.cpp` (876 files) and `include/**/*.hpp` (1 file) via generated explicit project items; the existing `learn::topic` self-registration mechanism.
- Produces: `build/vs/x64/Release/learn_cpp.exe` (and Debug in Task 2); a loadable `LearnCpp.slnx` for VS/MSBuild.

- [ ] **Step 1: Create the vcxproj**

Create `vs/learn_cpp.vcxproj` with exactly this content (CRLF line endings are enforced by `.gitattributes`; the Write tool writes LF and git/pre-commit will normalize — see Step 4):

```xml
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>

  <PropertyGroup Label="Globals">
    <VCProjectVersion>17.0</VCProjectVersion>
    <Keyword>Win32Proj</Keyword>
    <ProjectGuid>{44b4c0d0-2a55-4a73-b803-cc04213d600b}</ProjectGuid>
    <RootNamespace>learn_cpp</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
  </PropertyGroup>

  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />

  <PropertyGroup Condition="'$(Configuration)'=='Debug'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>ClangCl</PlatformToolset>
    <CharacterSet>MultiByte</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)'=='Release'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>ClangCl</PlatformToolset>
    <CharacterSet>MultiByte</CharacterSet>
  </PropertyGroup>

  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings" />
  <ImportGroup Label="PropertySheets">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>

  <PropertyGroup Label="UserMacros" />

  <PropertyGroup>
    <RepoRoot>$([System.IO.Path]::GetFullPath('$(MSBuildThisFileDirectory)..\'))</RepoRoot>
    <OutDir>$(RepoRoot)build\vs\$(Platform)\$(Configuration)\</OutDir>
    <IntDir>$(RepoRoot)build\vs\$(Platform)\$(Configuration)\obj\</IntDir>
  </PropertyGroup>

  <ItemDefinitionGroup>
    <ClCompile>
      <LanguageStandard>stdcpp23</LanguageStandard>
      <ConformanceMode>true</ConformanceMode>
      <WarningLevel>Level4</WarningLevel>
      <PrecompiledHeader>NotUsing</PrecompiledHeader>
      <ExceptionHandling>Sync</ExceptionHandling>
      <AdditionalIncludeDirectories>$(RepoRoot)include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <AdditionalOptions>-Wno-unused-command-line-argument -Wno-unused-parameter -Wno-microsoft-include %(AdditionalOptions)</AdditionalOptions>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
    </Link>
  </ItemDefinitionGroup>

  <ItemDefinitionGroup Condition="'$(Configuration)'=='Debug'">
    <ClCompile>
      <Optimization>Disabled</Optimization>
      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>
      <DebugInformationFormat>OldStyle</DebugInformationFormat>
    </ClCompile>
    <Link>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>

  <ItemDefinitionGroup Condition="'$(Configuration)'=='Release'">
    <ClCompile>
      <Optimization>MaxSpeed</Optimization>
      <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>
    </ClCompile>
    <Link>
      <GenerateDebugInformation>false</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>

  <ItemGroup>
    <ClCompile Include="..\src\**\*.cpp" />
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="..\include\**\*.hpp" />
  </ItemGroup>

  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets" />
</Project>
```

- [ ] **Step 2: Create the slnx**

Create `LearnCpp.slnx` at the repo root with exactly this content:

```xml
<?xml version="1.0" encoding="utf-8"?>
<Solution Version="1.0" Description="LearnCpp - VS2026 clang-cl C++23preview (independent of CMake)">
  <Configurations>
    <BuildType Name="Debug" />
    <BuildType Name="Release" />
    <Platform Name="x64" />
  </Configurations>
  <Project Path="vs/learn_cpp.vcxproj" DisplayName="learn_cpp" />
</Solution>
```

- [ ] **Step 3: Add `vcxproj` to the pre-commit `mixed-line-ending` exclude**

In `.pre-commit-config.yaml`, change the `mixed-line-ending` hook's `exclude` from:

```yaml
      - id: mixed-line-ending
        args: [--fix=lf]
        exclude: \.(bat|cmd|ps1)$
```

to:

```yaml
      - id: mixed-line-ending
        args: [--fix=lf]
        exclude: \.(bat|cmd|ps1|vcxproj)$
```

This makes the hook respect `.gitattributes` (`*.vcxproj eol=crlf`) and keeps CI's `pre-commit run --all-files` green once the vcxproj is committed.

- [ ] **Step 4: Pre-fix line endings / formatting on the new files**

Run pre-commit on exactly the new/modified files so any auto-fixes (trailing whitespace, final newline) are applied before staging:

```bash
pre-commit run --files vs/learn_cpp.vcxproj LearnCpp.slnx .pre-commit-config.yaml
```

Expected: all hooks `Passed` (clang-format `Skipped` — no c/cpp files; check-yaml `Passed` on `.pre-commit-config.yaml`; codespell `Passed`; mixed-line-ending `Skipped` on the vcxproj now that it is excluded, `Passed` on the others). If any hook reports `Failed - files were modified`, re-run the command once more; it should then pass. If codespell flags a word, fix it in the file or add the token to `.codespell-ignore` and re-run.

- [ ] **Step 5: Build Release via the slnx (clang-cl + VS2026)**

From the worktree root, build the solution with MSBuild inside a vcvars64 shell. Run this in the PowerShell tool with `run_in_background: true` (the 876 tiny TUs compile sequentially — clang-cl has no `/MP` — and may take ~5–7 min; background avoids the 10-min tool timeout):

```powershell
cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" && msbuild LearnCpp.slnx -p:Configuration=Release -p:Platform=x64 -m -nologo -v:m 2>&1' > build-slnx-release.log
```

Then wait for the background task to finish (it re-invokes you on completion), and inspect the tail of `build-slnx-release.log`. Expected: near the end, `Build succeeded. 0 Error(s)` and a line reporting `876 ClCompile` items and `1 Link`. The exe is at `build/vs/x64/Release/learn_cpp.exe`.

Fallback if `msbuild LearnCpp.slnx` is not accepted as a solution by this MSBuild: build the vcxproj directly (it uses `RepoRoot`, so it does not need the slnx):

```powershell
cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" && msbuild vs\learn_cpp.vcxproj -p:Configuration=Release -p:Platform=x64 -m -nologo -v:m 2>&1' > build-slnx-release.log
```

If the build fails, read the log, fix the vcxproj (common issues: a wrong property value, a flag clang-cl rejects — note it rejects `/Yc`/PCH flags so keep `PrecompiledHeader=NotUsing`), and re-run.

- [ ] **Step 6: Confirm `/std:c++23preview` is actually passed**

Grep the build log for the standard switch and the clang-cl driver:

```bash
grep -i "std:c++23preview\|clang-cl" build-slnx-release.log | head
```

Expected: at least one line showing `clang-cl.exe` invoked with `/std:c++23preview`. If the flag is absent, `LanguageStandard=stdcpp23` is not mapping — add `/std:c++23preview %(AdditionalOptions)` to the `AdditionalOptions` string in the vcxproj and rebuild.

- [ ] **Step 7: Smoke-run the Release exe**

```powershell
$exe = "build/vs/x64/Release/learn_cpp.exe"
& $exe | Select-Object -First 5
$count = (& $exe | Measure-Object -Line).Lines
if ($count -lt 800) { Write-Error "expected >=800 topic lines, got $count"; exit 1 }
Write-Host "OK ($count lines from learn_cpp)"
```

Expected: the first lines are topic ids like `part2/stage01/section01/...`; final line `OK (<N> lines from learn_cpp)` with N >= 800 (should be ~874). If N < 800, some topic .obj was not linked — confirm the generated `ClCompile` list contains all 876 files (check the build log's `ClCompile` count).

- [ ] **Step 8: Stage and commit**

```bash
git add vs/learn_cpp.vcxproj LearnCpp.slnx .pre-commit-config.yaml
git commit -m "Add independent LearnCpp.slnx (VS2026 clang-cl, C++23preview)

A CMake-independent XML solution + vcxproj that builds learn_cpp with VS2026's
bundled clang-cl 22.1.3 at /std:c++23preview (LanguageStandard=stdcpp23). The
vcxproj lists generated explicit src/**/*.cpp + include/**/*.hpp items, writes to build/vs/, and reads
no CMake output. Adds vcxproj to pre-commit's mixed-line-ending exclude so the
hook respects .gitattributes (vcxproj eol=crlf) and CI pre-commit stays green.

Co-Authored-By: Claude <noreply@anthropic.com>"
```

Expected: pre-commit passes (it was pre-run in Step 4) and the commit is created.

---

### Task 2: Build Debug config and smoke-run (verification checkpoint)

**Files:** none new (only edit `vs/learn_cpp.vcxproj` if Debug surfaces a different error).

**Interfaces:** produces `build/vs/x64/Debug/learn_cpp.exe` to confirm both configurations link and run.

- [ ] **Step 1: Build Debug via the slnx**

Run in the PowerShell tool with `run_in_background: true`:

```powershell
cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" && msbuild LearnCpp.slnx -p:Configuration=Debug -p:Platform=x64 -m -nologo -v:m 2>&1' > build-slnx-debug.log
```

After the background task completes, inspect the tail. Expected: `Build succeeded. 0 Error(s)`. If it fails, read the log — Debug uses `/MDd /Od /Z7`, which can surface different issues than Release; fix the vcxproj and rebuild, then `git add vs/learn_cpp.vcxproj && git commit -m "Fix Debug config of learn_cpp.vcxproj"` if a fix was needed.

- [ ] **Step 2: Smoke-run the Debug exe**

```powershell
$exe = "build/vs/x64/Debug/learn_cpp.exe"
& $exe | Select-Object -First 5
$count = (& $exe | Measure-Object -Line).Lines
if ($count -lt 800) { Write-Error "expected >=800 topic lines, got $count"; exit 1 }
Write-Host "OK Debug ($count lines)"
```

Expected: `OK Debug (<N> lines)` with N >= 800. No commit if both Release (Task 1) and Debug build and run clean.

---

### Task 3: Add the `slnx-msbuild` job to Windows CI

**Files:**
- Modify: `.github/workflows/windows-ci.yml` (add a second job under `jobs:`)

**Interfaces:** consumes `LearnCpp.slnx` + `vs/learn_cpp.vcxproj` (from Task 1). Produces a CI job that builds the slnx with the same VS2026 env as the cmake job.

- [ ] **Step 1: Add the new job**

In `.github/workflows/windows-ci.yml`, the file currently has one job `build:` under `jobs:`. Add a second job `slnx-msbuild:` immediately after the `build` job's last step (the `Run pre-commit` step) and before the file ends. The new job (note: 2-space indent for YAML, matching the file):

```yaml
  slnx-msbuild:
    name: Windows x64 slnx (clang-cl + MSBuild, independent of CMake)
    runs-on: windows-2025-vs2026
    timeout-minutes: 60
    steps:
      - name: Checkout
        uses: actions/checkout@v7

      - name: Setup MSVC dev environment (VS2026, x64)
        uses: ilammy/msvc-dev-cmd@v1
        with:
          arch: x64
          vsversion: 18.0

      - name: Put VS2026 bundled clang-cl on PATH
        # Mirrors the cmake job: the ClangCl MSBuild toolset auto-finds clang-cl
        # via VsInstallRoot, but putting it on PATH also lets us verify it here.
        run: |
          $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
          $vsPath = & $vswhere -latest -version "[18.0,19.0)" -property installationPath
          if (-not $vsPath) {
            Write-Error "VS2026 (= VS18) not found on this runner via vswhere"
            exit 1
          }
          $llvmBin = Join-Path $vsPath "VC\Tools\Llvm\x64\bin"
          if (-not (Test-Path "$llvmBin\clang-cl.exe")) {
            Write-Error "clang-cl.exe missing under $llvmBin"
            exit 1
          }
          echo $llvmBin | Out-File -FilePath $env:GITHUB_PATH -Encoding utf8 -Append

      - name: Verify tools
        run: |
          (Get-Command clang-cl).Source
          clang-cl --version
          (Get-Command MSBuild).Source
          MSBuild -version | Select-Object -First 1

      - name: Build LearnCpp.slnx (Release, x64, clang-cl)
        run: MSBuild LearnCpp.slnx -p:Configuration=Release -p:Platform=x64 -m -nologo

      - name: Smoke-run learn_cpp (list registered topics)
        run: |
          $exe = "build/vs/x64/Release/learn_cpp.exe"
          & $exe | Select-Object -First 5
          $count = (& $exe | Measure-Object -Line).Lines
          if ($count -lt 800) { Write-Error "expected >=800 topic lines, got $count"; exit 1 }
          Write-Host "OK ($count lines from learn_cpp)"
```

The job uses `defaults.run.shell: pwsh` already set at the workflow top level, so the PowerShell `run:` blocks are correct. It runs in parallel with `build` (GitHub Actions runs jobs concurrently by default).

- [ ] **Step 2: Validate the YAML and run pre-commit**

```bash
pre-commit run --files .github/workflows/windows-ci.yml
```

Expected: `check-yaml ... Passed`, `codespell ... Passed`, `trailing-whitespace / end-of-files / mixed-line-ending Passed`. If check-yaml fails, fix the indentation (2-space, jobs are at 2 spaces, steps at 4, run-block content at 6 — match the existing `build` job).

- [ ] **Step 3: Stage and commit**

```bash
git add .github/workflows/windows-ci.yml
git commit -m "CI: add parallel slnx-msbuild job for Windows (VS2026 clang-cl)

New job on windows-2025-vs2026 mirrors the cmake job's VS2026 setup
(ilammy/msvc-dev-cmd v18.0 + bundled clang-cl 22.x) and builds LearnCpp.slnx
with MSBuild at Release/x64, then smoke-runs learn_cpp.exe (>=800 topics).
Independent of CMake; runs in parallel with the existing cmake build job.

Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

### Task 4: Push the branch and open the PR

**Files:** none (git operations only).

- [ ] **Step 1: Push the worktree branch**

```bash
git push -u origin worktree-add-learncpp-slnx
```

Expected: the branch is pushed to `origin` (the `deku2026/LearnCpp` fork remote). If `git remote -v` shows no `origin`, stop and ask the user which remote/branch to push to.

- [ ] **Step 2: Open the PR with `gh`**

```bash
gh pr create --base main --head worktree-add-learncpp-slnx \
  --title "Add independent LearnCpp.slnx (VS2026 clang-cl, C++23preview) + Windows CI job" \
  --body "$(cat <<'EOF'
## What

Adds a **CMake-independent** Visual Studio XML solution that builds `learn_cpp`
with VS2026's bundled **clang-cl 22.1.3** at **`/std:c++23preview`**, plus a
parallel Windows CI job that builds it with the same VS2026 environment.

## Files

- `LearnCpp.slnx` — minimal XML solution at the repo root (Debug + Release, x64).
- `vs/learn_cpp.vcxproj` — `ClangCl` toolset, `LanguageStandard=stdcpp23`
  (emits `/std:c++23preview`), generated explicit `src/**/*.cpp` +
  `include/**/*.hpp` items, writes to `build/vs/`.
- `vs/learn_cpp.vcxproj.filters` — generated VS Solution Explorer tree matching
  the explicit project item Include strings.
- `.pre-commit-config.yaml` — add `vcxproj` to `mixed-line-ending` exclude so the
  hook respects `.gitattributes` (`*.vcxproj eol=crlf`) and CI's
  `pre-commit run --all-files` stays green.
- `.github/workflows/windows-ci.yml` — new `slnx-msbuild` job (parallel to the
  existing cmake `build` job, which is unchanged).

## Independence from CMake

The vcxproj compiles the sources directly with clang-cl. It reads no
`CMakeLists.txt`, `CMakePresets.json`, or `compile_commands.json`, and writes to
`build/vs/` (separate from CMake's `build/<preset>/`). New topic `.cpp` files are
picked up after re-running `scripts/generate-vs-filters.ps1`.

## Local verification

Built and smoke-run locally with VS2026 (v18.7) + clang-cl 22.1.3 for both
**Release** and **Debug** (x64): `MSBuild LearnCpp.slnx` produces
`build/vs/x64/<Config>/learn_cpp.exe`, which lists >=800 registered topics.

## Notes

- No `-Werror` on the slnx build (the cmake CI job continues to gate `-Werror`).
- Build is sequential per TU (clang-cl has no `/MP`); ~5–7 min, within the 60-min
  CI timeout. `UseMultiToolTask` can be added later if faster builds are needed.

🤖 Generated with [Claude Code](https://claude.com/claude-code)
EOF
)"
```

Expected: `gh` creates the PR and prints its URL. If `gh` is not authenticated, stop and tell the user to run `! gh auth login`.

- [ ] **Step 3: Report**

Report the PR URL to the user. Note that CI (both the cmake `build` job and the new `slnx-msbuild` job) will run on the PR; monitor them if desired.

---

## Self-Review (run after writing the plan)

- **Spec coverage:** Goal ✓ (Task 1–2 build, Task 3 CI). All vcxproj settings from the spec table ✓ (Task 1 Step 1). `.slnx` structure ✓ (Task 1 Step 2). CI job ✓ (Task 3). Local verification ✓ (Tasks 1–2). Independence ✓ (Global Constraints + Task 1). No `-Werror` ✓ (Global Constraints + vcxproj omits `TreatWarningAsError`). Generated `.filters` ✓ (File Structure). Pre-commit/line-ending fix ✓ (Task 1 Step 3) — this was a gap in the spec discovered via `.gitattributes` inspection; required so the existing cmake CI job's `pre-commit --all-files` does not break on the new CRLF vcxproj.
- **Placeholder scan:** No TBD/TODO; every code step has the actual file content; every command has expected output.
- **Type consistency:** `LanguageStandard=stdcpp23` used consistently (corrected from the spec's `stdcpp23preview`); `RepoRoot` property used consistently for include/out/int dirs; generated explicit `..\src\...` items consistent with the 876-file source tree.
