# R-stage04 · 类与对象模型基础 交叉 Review

**日期:** 2026-07-15
**Worktree:** `C:\MyFile\ArcForges\LearnCpp\.worktrees\fill-cpp-examples`
**文档权威:** `ArchitectureDesign/CppStudy/第2部分-阶段4-类与对象模型基础.md`
**代码根:** `src/cpp/part2_stage04_classes_and_object_model_basics/`
**Topic 数:** 47（全覆盖，无新增文件；topic id 未改）

---

## 1. 文档概念清单 L（步骤 → 要求）

| 锚点 | 概念 / 验收 | 标注 |
|------|-------------|------|
| 1.1–1.2 | 成员/访问控制；`const` 成员函数；`this` 链式 | 🟢 |
| 1.3 | 初始化列表（声明序）；NSDMI；委托构造 | 🟢 |
| 1.4 | `explicit` 单参构造防隐式转换 | 🟢 |
| 2.x | RAII：构造获取/析构释放；异常/早退安全；FileHandle 验收 | ⭐ |
| 3.1–3.2 | 六个特殊成员；`=default`/`=delete` | ⭐ |
| 3.3 | 抑制规则表（析构/拷贝抑移动；移动删拷贝） | ⭐ |
| 3.4–3.5 | Rule of 0 / 3 / 5（C.21） | ⭐ |
| 4.x | ref-qualified `&`/`&&`；四份重载样板问题 | ⭐ |
| 5.1–5.2 | deducing this 基础；一模板消四重载（验收） | 🆕⭐ |
| 5.3 | 简化 CRTP | 🆕⭐ |
| 5.4 | 递归 lambda；按值传 this | 🆕 |
| 5.5 | 不能 static/virtual/再叠 cv-ref；体内无 this（验收） | ⚠️ |
| 6.1–6.4 | 运算符成员/非成员；`<<`；`[]`/`()`；转换 / explicit 转换 | 🟢 |
| 6.5 | static `operator()`/`[]`；多维下标 `m[i,j]`（验收） | 🆕 |
| 7.x | `<=>` 一行六比较；ordering 类别；自定义 `<=>` 不自动 `==` | ⭐ |
| 8.1–8.5 | static / mutable / friend / 嵌套类 / 字面值类型 | 🟢 |
| 8.6 | 继承构造 CTAD（P2582） | 🆕 |
| 9.1–9.2 | public 继承；virtual；override/final | 🟢⭐ |
| 9.3 | 纯虚/抽象；虚析构验收 | ⭐ |
| 9.4 | 构造/析构中虚调用不派发到派生 | ⚠️ |
| 10–11 | Qt 对照；UB；验收 | 🔶 |

---

## 2. 覆盖矩阵（L × 主要 F）

| 文档点 | 主挂靠 topic | 覆盖 | 备注 |
|--------|--------------|------|------|
| 成员/访问/const | `section01/members_and_access_control` | **PASS** | |
| this 指针 | `section01/this_pointer` | **PASS** | |
| 委托构造 / NSDMI / explicit | constructor_delegation, nsdmi_*, explicit_constructor | **PASS** | |
| 聚合 vs 非聚合 | `aggregate_vs_non_aggregate` | **PASS** | |
| RAII 句柄 / 异常配对 | `raii_resource_handle`, `raii_exception_safety_pairing` | **PASS+** | Guard 计数 + 析构逆序 |
| 特殊成员单题 | default/copy/move/dtor 各文件 | **PASS** | |
| `=default`/`=delete` | `defaulted_and_deleted_members` | **PASS** | |
| 抑制规则 | `suppression_rules` | **STRONG** | 移动退化拷贝可观察（源仍 5） |
| Rule of 0/3/5 | rule_of_* | **PASS** | Ro5 含 move-only FileToken |
| ref-qualified | `section06/ref_qualified_member_functions` | **PASS** | |
| 消四重载 | `collapse_ref_qualified_overloads` | **STRONG** | Classic 四份 vs Widget 模板 |
| deducing this 基础 | `explicit_object_parameter_basics` | **PASS+** | 本轮：`x_ref` 转发 |
| 限制 | `deducing_this_restrictions` | **PASS** | 注释非法组合 + 多态并存 |
| 按值 this / CRTP / 递归 | pass_this_by_value, simplified_crtp, recursive_lambda | **PASS** | |
| 算术 / 流 / 下标调用 | section05 多个 | **PASS** | |
| 转换 / explicit 转换 | conversion_*, explicit_conversion_* | **PASS+** | 去掉 assert(true) |
| static op / 多维下标 | static_operator_*, multidim_* | **PASS** | 3D + C 数组逗号弃用注释 |
| spaceship / defaulted | comparison_*, defaulted_comparison_* | **PASS** | 自定义 `<=>` 需手写 `==`；NaN partial |
| static/mutable/friend/nested/literal | section06 | **PASS** | |
| 继承 CTAD | `inherited_constructor_ctad_cpp23` | **PASS** | 显式 guide 保可移植 + P2582 语义 |
| public 继承 / 切片 | `public_inheritance` | **PASS** | 切片 speak 退化 |
| override / final | override_specifier, final_specifier | **PASS** | |
| 纯虚抽象 | `pure_virtual_and_abstract` | **PASS** | 纯虚可有定义；is_abstract |
| 虚析构 | `virtual_destructor` | **PASS+** | unique_ptr；protected 策略；has_virtual_destructor |
| 虚函数 + 构造期 | `virtual_functions` | **PASS+** | **本轮补 9.4 日志断言** |

**等级汇总:** FAIL=0；原缺口「构造/析构虚调用」已补；WEAK 已抬升。

---

## 3. 本轮已做修改

| 文件 | 变更摘要 |
|------|----------|
| `section08/.../virtual_functions.cpp` | 构造期 `boot()` 日志证明只调 `Base::boot`；析构 `shutdown`；非虚静态绑定对照 |
| `section08/.../virtual_destructor.cpp` | 虚析构 + 五件套 default；`has_virtual_destructor`；protected 非虚策略 |
| `section04/.../explicit_object_parameter_basics.cpp` | `template x_ref(this Self&&)` 与 collapse 衔接 |
| `section05/.../explicit_conversion_operators.cpp` | 去掉 `assert(true)`，改为 contextual bool 旗标 |
| `section02/.../raii_exception_safety_pairing.cpp` | 去掉空洞 `assert(true)`，明确 lock_guard 路径 |

**编译抽检:** 上述变更 .obj 在 `build/review-check` 全部成功。

---

## 4. Primer 级质量抽检（5 个关键文件）

| Topic | 深度 | 边界/断言 | 结论 |
|-------|------|-----------|------|
| `suppression_rules` | 全隐式→析构抑移动→移动删拷贝→FixMoves | `is_trivially_move_*` + 源仍 5 | STRONG |
| `rule_of_five` | IntBuffer 五件套 + FileToken 只移动 | 深拷贝/移动后 size | STRONG |
| `collapse_ref_qualified_overloads` | Classic 四份 vs 一模板 | decltype 引用类别 | STRONG |
| `virtual_destructor` | new/delete 顺序；unique_ptr；protected | log `+D~D~B` | STRONG |
| `virtual_functions` | 多态容器 + **构造期虚调用** | log 无 Derived::boot | STRONG |

---

## 5. 验收清单对照

| 验收项（文档） | 证据 | 状态 |
|----------------|------|------|
| RAII 包装类（文件/锁） | `raii_resource_handle`, exception pairing | ✅ |
| 定义析构为何抑制移动 | `suppression_rules` | ✅ |
| Rule of 0 为何默认 | `rule_of_zero` | ✅ |
| deducing this 消四重载 | `collapse_ref_qualified_overloads` | ✅ |
| deducing this 不能 virtual | `deducing_this_restrictions` | ✅ |
| 多维下标 `operator[](i,j)` | `multidim_operator_subscript_cpp23` | ✅ |
| `<=>` 一行生成比较 | `defaulted_comparison_operators` | ✅ |
| 基类析构必须 virtual | `virtual_destructor` | ✅ |
| 构造中虚调用不派发派生 | `virtual_functions`（本轮补强） | ✅ |

---

## 6. 残留 / 可选后续

1. `pass_this_by_value`（58 行）、`conversion_operators`（59）、`static_member`/`literal_type`（60）仍紧凑但三点齐全。
2. `inherited_constructor_ctad_cpp23` 用显式 deduction guide 保证可移植；真·P2582 无 guide 行为依赖工具链，已在注释标明。
3. 多继承/虚继承/vtable 布局按文档归第 6 部分支线，本 stage 不展开（切片仅作 expert 提示）。
4. 全量 `learn_cpp` 链接与 windows-debug 预设修复留给 R7。

---

## 7. 结论

Stage04 文档三大主题（RAII、特殊成员联动、deducing this + 运算符/比较/继承）均有可运行证据；本轮补强**构造期虚调用（9.4）**与虚析构/显式对象参数细节，消除弱断言。
**Stage04 review：完成。**
