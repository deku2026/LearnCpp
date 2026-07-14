# Android Root Gradle Unify Implementation Plan

> **For agentic workers:** Inline execution in current session.

**Goal:** One root Gradle manages JVM (Kotlin/Java labs) + all Android HelloWorld labs; fix IDEA white-code by making Android real Gradle modules.

**Architecture:** Multi-project: root = Kotlin JVM application; `src/android/*` = AGP application modules. Physical path under `src/` for parity with `src/kotlin` / `src/java` / `src/cpp`.

**Tech Stack:** Gradle 9.6.1, AGP 9.2.1, Kotlin 2.4.0, Compose BOM 2026.06.01, JDK 25 toolchain / JVM 21 Android bytecode.

## Global Constraints

- Single root `settings.gradle.kts` + `gradlew` only (no nested wrappers).
- Android modules live at `src/android/<lab>/`.
- JDK 25 toolchain; Android `jvmTarget` 21; compileSdk/targetSdk 37; minSdk 26.
- Keep existing LearnKotlin / LearnJava `.run` configs.
- One `.run` per Android lab (Gradle assemble + Android App).
- Verify with `assembleDebug` for all Android modules before commit.

## Why IDEA shows white Kotlin

Standalone `android/*` projects are not linked in root `.idea/gradle.xml` (only `$PROJECT_DIR$`). Opening the monorepo never imports Android modules → no classpath / Android facet → no highlighting. KMP plugin alone cannot fix missing Gradle module linkage.

## Tasks

### Task 1: Root Gradle Android-capable settings

- Modify: `settings.gradle.kts`, `build.gradle.kts`, `gradle.properties`
- Create: `gradle/libs.versions.toml`, root `local.properties` (gitignored)

### Task 2: Migrate labs to `src/android`

- Move each former `android/<lab>/app/*` → `src/android/<lab>/`
- Module `build.gradle.kts` applies AGP + Compose via version catalog
- Delete top-level `android/` standalone projects

### Task 3: `.run` configurations

- Keep: LearnKotlin*, LearnJava*
- Add: one Gradle run + one Android App run per lab module

### Task 4: Verify + commit

- `./gradlew compileKotlin` (root)
- `./gradlew assembleDebug` (all android modules)
- Local commit on `feature/android-mobile-labs`
