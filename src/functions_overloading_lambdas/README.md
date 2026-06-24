# 阶段 3 · 函数 / 重载 / Lambda

- 路线图: `第2部分-阶段3-函数-重载-lambda.md`
- 源文档: `C:\MyFile\ArcForges\ArchitectureDesign\CppStudy$doc`
- 主路线索引: `C:\MyFile\ArcForges\ArchitectureDesign\CppStudy\Cpp-Modern-完整学习路线图-C++23.md`

## 目标

掌握函数全套, 重点是 lambda 及其 '被改写成闭包类' 的本质 + C++23 的 static operator() / 递归 lambda / move_only_function.

## 主题子目录

```bash
ls src/functions_overloading_lambdas
```

每个子目录代表路线图里的一个"大项", 子目录内的 `*.cpp` 是逐个"小项"的占位符
(`GTEST_SKIP()`, 编译能过但被标 SKIPPED). `import std` 之外的 `module` 相关内容已剔除.

## 学习节奏 (每个 `.cpp`)

1. 读 cppreference 对应页 (英文站第一手) + 路线图本节;
2. 在 `godbolt.org` 看汇编, `cppinsights.io` 看编译器改写;
3. 写最小例子, 把 `GTEST_SKIP()` 换成 `EXPECT_*` / `ASSERT_*` 断言;
4. 跑这一阶段: `ctest --preset windows-debug --output-on-failure -R 'functions_overloading_lambdas__'`;
5. 留意 Qt / 旧 C++ 对照 + UB 边界, 用 ASan/UBSan/TSan 验证;
6. 想保留要复用的工具型代码就在同级目录加 `.h`, CMake 会 GLOB 进同一个测试 exe.

## 直接构建

```bash
# Windows (须在 vcvars64 prompt, 或 scripts/dev-shell.cmd)
cmake --preset windows-debug
cmake --build --preset windows-debug
ctest --preset windows-debug --output-on-failure -R 'functions_overloading_lambdas__'
```

## 添加新占位

复制任意一份 `.cpp`, 改:
- 文件名 -> 新的"小项" slug;
- 顶部 `Item :` 注释;
- `TEST(functions_overloading_lambdas__<topic>, <new_item>)` 里的 `<new_item>`.

CMake `GLOB_RECURSE CONFIGURE_DEPENDS` 会在下一次 `cmake --build` 时自动接上.
