# Part 6 · Branch F · 内存模型与并发 (深水原子/无锁)

- 路线图源文档: `第6部分-支线F-内存模型与并发.md` (在 `C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/` 下找原始中文版)
- 主路线索引: `C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/Cpp-Modern-完整学习路线图-C++23.md`

## 目标

C++ 内存模型 / memory_order 各级别 / 无锁数据结构 / CAS / ABA / atomic_ref / false sharing / 内存屏障 / 双检锁.

## 子目录 (sectionNN_xxx, 按文档大项顺序)

```bash
ls src/part6_branch_f_memory_model_and_concurrency
```

子目录里的 `*.cpp` 是逐个小项的占位符 (`GTEST_SKIP()` 兜底). `module` 主体剔除,
`import std` 例外保留在 `part4_engineering_system/section07_import_std_exception/`.

## 一个占位的学习节奏

1. 读 cppreference 对应页 (英文站第一手) + 路线图本节;
2. 在 `godbolt.org` 看汇编, `cppinsights.io` 看编译器改写;
3. 写最小例子, 把 `GTEST_SKIP()` 换成 `EXPECT_*` / `ASSERT_*` 断言;
4. 跑该范围: `ctest --preset windows-debug --output-on-failure -R 'part6_branch_f'`;
5. 留意 Qt / 旧 C++ 对照 + UB 边界, 用 ASan/UBSan/TSan 复查.

## 测试命名约定

- `TEST(<stage_short>_<section_short>, <item>)`
- 例如 `TEST(part2_stage01_section01, main_and_program_structure)`;
- 在 gtest / ctest 输出里以 `partX_stageNN_sectionMM.<item>` 形式出现, 方便用 `--gtest_filter` 或 `ctest -R` 过滤.

## 添加新占位

复制任意一份 `.cpp`, 改:
- 文件名 -> 新的小项 slug;
- 顶部 `Item    :` 注释;
- `TEST(<stage_short>_<section_short>, <new_item>)` 里的 `<new_item>`.

CMake `file(GLOB_RECURSE CONFIGURE_DEPENDS "src/*.cpp")` 会在下次 `cmake --build` 时自动接上.
