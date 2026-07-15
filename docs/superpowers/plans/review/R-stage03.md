# R-stage03 · 函数 / 重载 / lambda 交叉 Review

**日期:** 2026-07-15
**Worktree:** `C:\MyFile\ArcForges\LearnCpp\.worktrees\fill-cpp-examples`
**文档权威:** `ArchitectureDesign/CppStudy/第2部分-阶段3-函数-重载-lambda.md`
**代码根:** `src/cpp/part2_stage03_functions_overloading_lambdas/`
**Topic 数:** 35（全覆盖，无新增文件；topic id 未改）

---

## 1. 文档概念清单 L（步骤 → 要求）

| 锚点 | 概念 / 验收 | 标注 |
|------|-------------|------|
| 步骤 1 | 传参：值 / `T&` / `const T&` / 指针；sink 按值+move；Qt COW 对照 | 🟢 |
| 步骤 2 | `inline` = 放宽 ODR，非强制内联；隐式 inline（类内/constexpr/模板） | ⭐ |
| 步骤 3.1 | 默认实参：从右向左、声明处一次、调用点求值 | 🟢 |
| 步骤 3.2–3.3 | 重载；返回类型不参与；决议三步；精确>提升>标准>用户定义；默认+重载歧义 | 🟢 |
| 步骤 4.1 | 尾置返回 `auto f()->T`；依赖参数 / 可读性 | ⭐ |
| 步骤 4.2–4.3 | `auto` 返回推导；多 return 同型；`decltype(auto)` 保留引用 | ⭐ |
| 步骤 5.1–5.2 | lambda 语法；闭包类本质；Insights 对照；默认 const `operator()` | ⭐ |
| 步骤 5.3 | 捕获全家桶：`[]/[&]/[=]` 混合 / `this` / `*this` / init-capture / pack | ⭐ |
| 步骤 5.4 | `mutable` | 🟢 |
| 步骤 5.5 | 引用捕获悬垂 UB + 修法（验收） | ⚠️ |
| 步骤 5.6–5.7 | 泛型 lambda C++14；模板 lambda C++20 | ⭐ |
| 步骤 5.8 | constexpr lambda；IIFE | 🟢 |
| 步骤 5.9 | 无捕获 → 函数指针 | 🟢 |
| 步骤 6.1 | static lambda / `static operator()`（P1169，验收） | 🆕⭐ |
| 步骤 6.2 | 可省 `()`（P1102）；lambda 属性 | 🆕 |
| 步骤 6.3 | 递归 lambda / deducing this（验收） | 🆕⭐ |
| 步骤 7.1–7.2 | 仿函数；`std::invoke`；`invoke_r` | ⭐ |
| 步骤 7.3–7.4 | `std::function`；`move_only_function`（验收） | 🆕⭐ |
| 步骤 7.5–7.6 | `bind_front`/`bind_back`；优先 lambda 而非 `bind` | 🟢 |
| 步骤 8 | `initializer_list`；可变参数模板预告；C 变参了解 | 🟢 |
| 步骤 10 | UB 清单 1–14 | ⚠️ |
| 步骤 11 | 验收清单 7 条 + 扩展练习 | 🟢 |

---

## 2. 覆盖矩阵（L × 主要 F）

| 文档点 | 主挂靠 topic | 覆盖 | 备注 |
|--------|--------------|------|------|
| 传参取舍 / sink | `section01/pass_by_value_reference_const_ref_pointer` | **PASS** | 四种形参 + sink + Qt COW |
| inline / ODR | `section01/inline_means_relax_odr` | **PASS** | |
| 默认实参 | `section01/default_arguments` | **PASS** | |
| 重载决议 | `section01/overload_resolution_intro` | **PASS** | 精确/提升/用户定义/歧义注释 |
| 尾置返回 | `section01/trailing_return_type` | **PASS** | |
| auto / decltype(auto) | `section01/auto_return_type_deduction_cpp14` | **PASS** | 含完美转发返回对比 |
| 闭包类 + 函数指针 | `section03/closure_class_in_insights` | **PASS** | 手写仿函数 + qsort + fp |
| 值捕获 | `section03/capture_by_value` | **PASS+** | 本轮加厚：显式捕获、大对象副本 |
| 引用捕获 / 悬垂 | `section03/capture_by_reference` | **PASS** | 不执行 UB；安全路径断言 |
| this / *this | `capture_this`, `capture_star_this_cpp17` | **PASS** | `[=]` 捕获 this 指针陷阱 |
| init-capture | `section03/init_capture_cpp14` | **PASS+** | unique_ptr move + thin fields |
| pack 捕获 | `section03/capture_pack_cpp20` | **PASS** | |
| mutable | `section03/mutable_lambda` | **PASS+** | gen/acc/const 对象 |
| 泛型 / 模板 lambda | `generic_lambda_cpp14`, `template_lambda_cpp20` | **PASS** | concepts + pack |
| constexpr / IIFE | `constexpr_lambda_cpp17`, `iife_immediately_invoked` | **PASS+** | consteval + primes |
| static lambda | `section04/static_operator_call_cpp23` | **PASS+** | 本轮加厚：fn ptr / Sq / feature-test |
| 可省 () | `section04/optional_parameter_list_cpp23` | **PASS+** | 属性/static/尾置 |
| lambda 属性 | `section04/attributes_on_lambda_cpp23` | **PASS+** | nodiscard/deprecated（编译有意告警） |
| 递归 lambda | `section04/deducing_this_recursive_lambda_cpp23` | **PASS+** | y_combinator 对照 + fact/sum |
| 尾置作用域修订 | `section04/trailing_return_scope_revision_cpp23` | **PASS** | |
| 仿函数 | `section05/functor_basics` | **PASS** | |
| invoke / invoke_r | `std_invoke_cpp17`, `std_invoke_r_cpp23` | **PASS** | |
| function | `section05/std_function` | **PASS** | bad_function_call |
| move_only_function | `section05/move_only_function_cpp23` | **PASS+** | 回调 vector + 限定符 + 空 UB |
| bind_front/back / vs lambda | 三个 bind* topic | **PASS** | |
| 变参三条路 | section02 三个文件 | **PASS** | |

**等级汇总:** FAIL=0；原 WEAK（短文件入门层）本轮抬到 PASS+；无缺失知识点需新建文件。

---

## 3. 本轮已做修改（增强现有文件，保留 topic id）

| 文件 | 变更摘要 |
|------|----------|
| `section03/.../capture_by_value.cpp` | 显式捕获列表、大对象副本独立性、EMC++ 默认捕获提示 |
| `section03/.../mutable_lambda.cpp` | 生成器 / 累加器 / 与 out 向量组合 |
| `section03/.../init_capture_cpp14.cpp` | thin field 捕获、move-only work lambda |
| `section03/.../constexpr_lambda_cpp17.cpp` | 泛型 constexpr、consteval 边界 |
| `section04/.../static_operator_call_cpp23.cpp` | 转函数指针、手写 static call op、`std::function` 装载、feature-test |
| `section04/.../deducing_this_recursive_lambda_cpp23.cpp` | fact、尾递归 sum、Y 组合子对照、带捕获计数 |
| `section04/.../attributes_on_lambda_cpp23.cpp` | static/constexpr/省略 () 组合；类型擦除边界说明 |
| `section04/.../optional_parameter_list_cpp23.cpp` | 属性省略 ()、尾置返回、C++20 对照注释 |
| `section05/.../move_only_function_cpp23.cpp` | `vector<move_only_function>` 回调列表（对接扩展练习 2） |

**编译抽检:** `build/review-check` 下上述 .obj 全部成功（`attributes` 因 `[[deprecated]]` 故意触发 1 条 -Wdeprecated）。

---

## 4. Primer 级质量抽检（5 个关键文件）

| Topic | 深度 | 边界/断言 | 结论 |
|-------|------|-----------|------|
| `overload_resolution_intro` | 入门选重载→提升 vs 标准→歧义注释 | assert + static_assert | STRONG |
| `auto_return_type_deduction_cpp14` | auto 丢引用 vs decltype(auto) | 地址相等断言 | STRONG |
| `capture_by_reference` | 当场用完 vs 存起来 | 不跑 UB；safe 值捕获 | PASS |
| `closure_class_in_insights` | 手写仿函数 + 类型唯一 + fp | qsort + Cmp | STRONG |
| `move_only_function_cpp23` | 只移动 + 限定 + 空检测 | vector 回调 | STRONG |

---

## 5. 验收清单对照（文档步骤 11.1）

| 验收项 | 证据 topic | 状态 |
|--------|------------|------|
| 各种捕获 + 泛型 lambda | capture_* + generic_lambda + init_capture | ✅ |
| 闭包类 / Insights | closure_class_in_insights | ✅ |
| 静态 lambda + 递归 lambda | static_operator_call + deducing_this_recursive | ✅ |
| 引用捕获悬垂 | capture_by_reference（文字+安全路径） | ✅ |
| move_only_function | move_only_function_cpp23 | ✅ |
| inline 真义 | inline_means_relax_odr | ✅ |
| 传参取舍 | pass_by_value_reference_const_ref_pointer | ✅ |

---

## 6. 残留 / 可选后续

1. `template_lambda_cpp20` / `std_invoke_r_cpp23` / `std_bind_back` 仍偏短（~56–57 行）但三点齐全，非 FAIL。
2. 引用捕获悬垂仍**不执行**悬垂调用（符合「不故意 UB」铁律）；依赖注释 + 安全对照。
3. `windows-debug` 预设曾因 ninja recompaction Permission denied 无法原地 regenerate；改用 `build/review-check` 验证。全量链接留给 R7。
4. 未新增 `.cpp`；未改任何 `learn::topic<"...", run>` id。

---

## 7. 结论

Stage03 文档概念全集已在现有 35 个 topic 中可运行落地；本轮重点把 C++23 lambda / 捕获 / move_only_function 等薄弱短文件抬到 PASS+，验收 7 条均可指到代码。
**Stage03 review：完成。**
