# Android CI Assemble Implementation Plan

> **For agentic workers:** Inline execution in current session.

**Goal:** CI compiles all Android lab modules (`assembleDebug`) so HelloWorld scaffolds stay green like LearnKotlin/LearnJava.

**Architecture:** After existing JDK 25 + Gradle JVM steps, install Android SDK cmdline tools + platform 36.1 + build-tools, then run root task `assembleAndroidDebug` (already depends on every `:android:*:assembleDebug`).

**Tech Stack:** GitHub Actions, `android-actions/setup-android@v3`, AGP 9.0.1, Gradle 9.6.1, compileSdk 36.1 / minSdk 24, Temurin JDK 25.

## Global Constraints

- Do not add nested Gradle wrappers; root `gradlew` only.
- `local.properties` stays gitignored; CI uses `ANDROID_HOME` from setup action.
- Mirror JVM CI placement: same jobs that already run `compileKotlin` / `compileJava`.
- Windows: only the CMake `build` job (not `slnx-msbuild`) — one Android compile per Windows workflow is enough.
- No emulator / instrumented tests; compile-only gate (same spirit as JVM compile steps).

## Project context (read-only)

| Area | Path / fact |
|---|---|
| Root Gradle | `settings.gradle.kts` includes `:learn` + dynamic `:android:<lab>` from `src/android/*` |
| Aggregate task | root `build.gradle.kts` → `assembleAndroidDebug` |
| Labs | 36 AGP application modules under `src/android/` (Compose HelloWorld placeholders) |
| Existing CI | `.github/workflows/{linux,macos,windows}-ci.yml` — C++ then JDK25 + Gradle JVM |
| Branch | `feature/android-mobile-labs` (labs already committed; no PR yet) |

## File map

| File | Change |
|---|---|
| `.github/workflows/linux-ci.yml` | Setup Android SDK + `./gradlew assembleAndroidDebug` |
| `.github/workflows/macos-ci.yml` | Same |
| `.github/workflows/windows-ci.yml` | Same on `build` job only |

## Tasks

### Task 1: Linux / macOS / Windows CI steps

After Smoke-run LearnJava (and before pre-commit where present):

```yaml
      - name: Setup Android SDK
        uses: android-actions/setup-android@v3
        with:
          packages: platform-tools platforms;android-36.1 build-tools;36.1.0

      - name: Assemble Android labs (debug)
        run: ./gradlew assembleAndroidDebug --stacktrace --no-daemon
```

Windows uses `.\gradlew.bat` and `shell: pwsh` (workflow default).

Package pins match local successful build (`android-36.1`, build-tools `36.1.0`).

### Task 2: Verify locally (optional smoke)

```bat
gradlew.bat :android:part02-stage01-types-nullability:assembleDebug --no-daemon
```

Expected: `BUILD SUCCESSFUL`.

### Task 3: Commit, push, open PR

- Commit message: CI compile gate for all Android lab modules.
- Push `feature/android-mobile-labs`, open PR into `main`.
- PR body: summarize Android labs + CI assembleDebug on three OS workflows.

## Self-review

1. Spec coverage: CI Android compile on three platforms — yes; no nested wrapper; no emulator.
2. Placeholders: none.
3. Consistency: task name `assembleAndroidDebug` matches root `build.gradle.kts`.
