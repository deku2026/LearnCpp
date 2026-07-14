# Spring Boot Labs (start.spring.io-aligned)

| Item | Value |
|---|---|
| Template | Minimal Spring Boot Web HelloWorld |
| Spring Boot | **4.1.0** |
| Java | **21** toolchain |
| Module | `:spring:<lab>` |
| Aggregate | `assembleSpringBoot` → all `bootJar` |
| Run | `.run/Spring__<lab>.run.xml` → `:spring:<lab>:bootRun` |

Source of truth: `ArchitectureDesign/SpringStudy` concrete guides (not the roadmap).

Open **repo root**, Gradle Sync, then Run `Spring: step01-startup-model`.

## Modules

| Module | Title | Port | Doc |
|---|---|---|---|
| `step01-startup-model` | Step 01 Startup Model | 8201 | `步骤1-启动模型-完整实施指南.md` |
| `step02-ioc-di` | Step 02 IoC & DI | 8202 | `步骤2-IoC与依赖注入-完整实施指南.md` |
| `step03-bean-lifecycle-scope` | Step 03 Bean Lifecycle Scope | 8203 | `步骤3-Bean生命周期与Scope与条件装配-完整实施指南.md` |
| `step04-config-properties` | Step 04 Config Properties | 8204 | `步骤4-配置体系与ConfigurationProperties-完整实施指南.md` |
| `step05-autoconfig-starter` | Step 05 Autoconfig Starter | 8205 | `步骤5-自动配置与Starter与调试-完整实施指南.md` |
| `step06-mvc-rest` | Step 06 MVC REST | 8206 | `步骤6-Spring-MVC与REST-完整实施指南.md` |
| `step07-validation-problem-details` | Step 07 Validation ProblemDetails | 8207 | `步骤7-Validation与ProblemDetails-完整实施指南.md` |
| `step08-aop-tx-events` | Step 08 AOP TX Events | 8208 | `步骤8-AOP与事务与事件预埋-完整实施指南.md` |
| `step09-data-access` | Step 09 Data Access | 8209 | `步骤9-数据访问最小集-完整实施指南.md` |
| `step10-security-min` | Step 10 Security Min | 8210 | `步骤10-Security挂载最小集-完整实施指南.md` |
| `step11-testing-testcontainers` | Step 11 Testing Testcontainers | 8211 | `步骤11-测试与Testcontainers-完整实施指南.md` |
| `step12-actuator-logging` | Step 12 Actuator Logging | 8212 | `步骤12-Actuator与日志入口-完整实施指南.md` |
| `part03-data-layer` | Part 03 Data Layer | 8213 | `第3部分-数据层深化-完整实施指南.md` |
| `part04-security-deep` | Part 04 Security Deep | 8214 | `第4部分-安全深化-完整实施指南.md` |
| `part05-api-modulith` | Part 05 API Modulith | 8215 | `第5部分-API设计与模块化单体-完整实施指南.md` |
| `part06-resilience` | Part 06 Resilience | 8216 | `第6部分-弹性与失败处理-完整实施指南.md` |
| `part07-messaging-async` | Part 07 Messaging Async | 8217 | `第7部分-消息与异步-完整实施指南.md` |
| `part08-distributed-cloud` | Part 08 Distributed Cloud | 8218 | `第8部分-分布式与SpringCloud-完整实施指南.md` |
| `part09-observability` | Part 09 Observability | 8219 | `第9部分-可观测与排障-完整实施指南.md` |
| `part10-deploy-delivery` | Part 10 Deploy Delivery | 8220 | `第10部分-部署与交付-完整实施指南.md` |
| `part11-performance-aot-native` | Part 11 Performance AOT Native | 8221 | `第11部分-性能与AOT与Native与源码-完整实施指南.md` |
| `part12-electives` | Part 12 Electives | 8222 | `第12部分-选学支线-完整六要素摘要.md` |
| `capstone01-rest-api` | Capstone 01 REST API | 8223 | `第13部分-Capstone1-生产级单体REST-API-完整实施指南.md` |
| `capstone02-modulith` | Capstone 02 Modulith | 8224 | `第14部分-Capstone2-模块化单体Modulith-完整实施指南.md` |
| `capstone03-cloud-native` | Capstone 03 Cloud Native | 8225 | `第15部分-Capstone3-云原生多服务-完整实施指南.md` |

## Build all

```powershell
.\gradlew.bat assembleSpringBoot
```
