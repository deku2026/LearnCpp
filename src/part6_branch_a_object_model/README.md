# Part 6 · Branch A · 对象模型

- 路线图源文档: `第6部分-支线A-对象模型.md` (在 `C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/` 下找原始中文版)
- 主路线索引: `C:/MyFile/ArcForges/ArchitectureDesign/CppStudy/Cpp-Modern-完整学习路线图-C++23.md`

## 目标

对象在内存里怎么活: 布局 / 对齐 / vtable / vptr / 多继承 / 虚继承 / 对象切片 / RTTI / EBO / [[no_unique_address]].

## 子目录 (sectionNN_xxx, 按文档大项顺序)

```bash
ls src/part6_branch_a_object_model
```

每个 `*.cpp` 是一个独立占位 (空 `run(argc, argv)` 体), 通过 `learn::topic<id, run>` 变量模板注册到全局 registry,
按需手填例子 — 就是 `int main(int, char**)` 的赤裸版本, 直接 `std::cout` / 抛异常 / 玩 argv, 别再让测试框架卡你.

## 一个占位的学习节奏

1. 读 cppreference 对应页 (英文站第一手) + 路线图本节;
2. 在 `godbolt.org` 看汇编, `cppinsights.io` 看编译器改写;
3. 写最小例子, 把空 `run()` 填上真实代码 (`cout` 看输出, `assert` 卡边界, 抛异常验异常路径都可);
4. 跑这一个 topic:
   ```
   build/windows-debug/bin/learn_cpp.exe part6/a/sectionNN/item_slug
   ```
   不带参数 `learn_cpp.exe` 会列出所有已注册 topic 方便挑;
5. 留意 Qt / 旧 C++ 对照 + UB 边界, 用 ASan/UBSan/TSan 复查.

## Topic 命名约定

每个 `.cpp` 在 `learn::topic<...>` 里的 id 形如 `part6/a/sectionMM/<item_slug>`,
跟文件路径一一对应, 例:
```
part6/a/section01/<item_slug>
```
没有重名风险 (registry 是 `std::map`, 重复会被静默丢掉, 自己留意).

## 添加新占位

复制任意一份 `.cpp`, 改:
- 文件名 -> 新的小项 slug;
- 顶部 `Item    :` / `Topic id :` 注释里的 slug;
- `learn::topic<"...", run>` 实例化里的 id 字符串.

CMake `file(GLOB_RECURSE CONFIGURE_DEPENDS "src/*.cpp")` 在下次 `cmake --build` 自动接上, 不用动 CMakeLists.
