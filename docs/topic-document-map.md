# LearnCpp topic 文档映射

src/ 下的 topic 源文件通过顶部 Doc: 元数据指向路线图文档。Doc: 保存的是实际文件名，不是英文别名。

外部文档根目录默认是：

    C:\MyFile\ArcForges\ArchitectureDesign\CppStudy

校验时可以通过 scripts/validate-topic-catalog.ps1 -CppStudyRoot <目录> 指定另一份文档副本。

## Stage 映射

| Stage | Topics | Doc: 文件名 |
|---|---:|---|
| part2_stage01_syntax_and_translation_model | 19 | 第2部分-阶段1-语法基础与翻译模型.md |
| part2_stage02_type_system_value_category_deduction | 53 | 第2部分-阶段2-类型系统-值类别与类型推导.md |
| part2_stage03_functions_overloading_lambdas | 35 | 第2部分-阶段3-函数-重载-lambda.md |
| part2_stage04_classes_and_object_model_basics | 47 | 第2部分-阶段4-类与对象模型基础.md |
| part2_stage05_copy_move_smart_pointers | 32 | 第2部分-阶段5-拷贝与移动语义-智能指针.md |
| part2_stage06_templates_and_generics | 40 | 第2部分-阶段6-模板与泛型编程.md |
| part2_stage07_compile_time_programming | 21 | 第2部分-阶段7-编译期编程.md |
| part2_stage08_control_flow_and_modern_syntax | 20 | 第2部分-阶段8-控制流与现代语法增强.md |
| part2_stage09_exceptions_error_handling_ub | 45 | 第2部分-阶段9-异常-错误处理与UB纪律.md |
| part2_stage10_stl_deep_dive | 132 | 第2部分-阶段10-STL深潜.md |
| part2_stage11_concurrency_and_threading | 35 | 第2部分-阶段11-并发与多线程.md |
| part2_stage12_coroutines | 32 | 第2部分-阶段12-协程.md |
| part2_stage14_keywords_and_features_index | 18 | 第2部分-阶段14-特性与关键字全表.md |
| part2_stage15_classic_idioms | 22 | 第2部分-阶段15-C++经典惯用法.md |
| part3_standard_library_systematized | 86 | 第3部分-标准库系统化.md |
| part4_engineering_system | 52 | 第4部分-工程系统.md |
| part6_branch_a_object_model | 16 | 第6部分-支线A-对象模型.md |
| part6_branch_b_lifetime_and_ownership | 29 | 第6部分-支线B-生命周期与所有权.md |
| part6_branch_c_memory_management | 29 | 第6部分-支线C-内存管理.md |
| part6_branch_d_name_lookup_overload_resolution | 23 | 第6部分-支线D-名字查找ADL与重载决议.md |
| part6_branch_e_templates_compile_time_system | 26 | Cpp-Modern-完整学习路线图-C++23.md |
| part6_branch_f_memory_model_and_concurrency | 21 | 第6部分-支线F-内存模型与并发.md |
| part6_branch_g_abi_compile_link_ub_performance | 41 | 第6部分-支线G-ABI编译链接UB性能.md |

合计：23 个 stage、874 个 topic。

## 支线 E 回退规则

独立文档 第6部分-支线E-模板与编译期系统.md 当前不存在。支线 E 的 26 个 topic 统一回退到：

    Cpp-Modern-完整学习路线图-C++23.md

并使用以下标题作为内容锚点：

    支线 E · 模板与编译期系统(深水 TMP)

因此支线 E topic 还必须带有：

    // Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)

该回退只提供路线图级骨架。填充示例时仍需按 topic 补充 cppreference、标准草案和 WG21 提案等一手资料。

## 范围约束

- 第 2 部分阶段 13 的 Modules 文档存在，但仓库没有对应 stage 源码目录，不据此新增 topic。
- 仓库仍以常规头文件和翻译单元构建；不得加入真实 import std; 或模块单元。
- Doc: 只负责定位 stage 主文档；精确小节应通过 Doc anchor: 表达，不能凭英文 slug 猜测中文标题。

## 校验

内容填充期间可以运行过渡校验：

    pwsh -NoProfile -File scripts/validate-topic-catalog.ps1 -AllowPlaceholders

全部 topic 填充完成后运行默认严格校验：

    pwsh -NoProfile -File scripts/validate-topic-catalog.ps1
