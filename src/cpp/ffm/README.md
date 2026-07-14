# learn_ffm

C ABI shared library for LearnJava FFM (JEP 454).

| Item | Value |
|---|---|
| CMake target | `learn_ffm` (SHARED) |
| Output | `build/<preset>/bin/learn_ffm.dll` |

## Build

```bat
scripts\build-ffm-native.cmd
```

Debug build if you need PDB.

## Run / native attach

**LearnJava Main**, program arg:

```text
part8/native/s04/ffm_overview_jep454
```

Console prints `pid=...`, then every 200ms calls `learn_ffm_mul_add_i32`.

VS Code: **Attach C++ (learn_ffm → java)** (`.vscode/launch.json`) → pick that pid.
Breakpoint in `learn_ffm.cpp` (e.g. `learn_ffm_mul_add_i32`); PDB: `build/windows-debug/bin`.
