# Spring Boot Labs Implementation Plan

> **For agentic workers:** Inline execution in current session on `feature/spring-boot-labs`.

**Goal:** One complete standalone Spring Boot 4.1 Hello World (Java) lab per SpringStudy concrete guide file; Gradle multi-project like Android; IDEA `.run` configs; CI compiles all modules; commit + push; **no PR**.

**Architecture:** Mirror `src/android/*` → `src/spring/*`. Root Gradle includes `:spring` container + dynamic `:spring:<lab>`. Each lab is an independent Boot application module (`bootJar` / `bootRun`). Aggregate task `assembleSpringBoot` → all `:spring:*:bootJar`.

**Tech Stack:** Spring Boot **4.1.0**, Java **21** toolchain (Boot mainline; CI JDK 25 + foojay can resolve 21), Gradle **9.6.1** (existing), `spring-boot-starter-web` only (HelloWorld).

## Global Constraints

- **HelloWorld only** — full mechanism code stays for later study; each module is a complete runnable app shell (like Android Compose placeholders).
- **One module per concrete guide** under `ArchitectureDesign/SpringStudy` (exclude roadmap only).
- **No nested Gradle wrappers** — root `gradlew` only.
- **No PR** — commit + push branch only.
- Do not break existing `:learn` (Kotlin/Java topics) or `:android:*`.
- Unique `server.port` per lab so parallel local runs do not collide.
- Packages under `learn.spring.*` (parallel to `learn.android.*` / `learnj`).

## Source inventory → modules (25)

| # | Guide file | Module dir | Package | Port |
|---|---|---|---|---|
| 1 | 步骤1-启动模型-完整实施指南.md | `step01-startup-model` | `learn.spring.step01` | 8201 |
| 2 | 步骤2-IoC与依赖注入-完整实施指南.md | `step02-ioc-di` | `learn.spring.step02` | 8202 |
| 3 | 步骤3-Bean生命周期与Scope与条件装配-完整实施指南.md | `step03-bean-lifecycle-scope` | `learn.spring.step03` | 8203 |
| 4 | 步骤4-配置体系与ConfigurationProperties-完整实施指南.md | `step04-config-properties` | `learn.spring.step04` | 8204 |
| 5 | 步骤5-自动配置与Starter与调试-完整实施指南.md | `step05-autoconfig-starter` | `learn.spring.step05` | 8205 |
| 6 | 步骤6-Spring-MVC与REST-完整实施指南.md | `step06-mvc-rest` | `learn.spring.step06` | 8206 |
| 7 | 步骤7-Validation与ProblemDetails-完整实施指南.md | `step07-validation-problem-details` | `learn.spring.step07` | 8207 |
| 8 | 步骤8-AOP与事务与事件预埋-完整实施指南.md | `step08-aop-tx-events` | `learn.spring.step08` | 8208 |
| 9 | 步骤9-数据访问最小集-完整实施指南.md | `step09-data-access` | `learn.spring.step09` | 8209 |
| 10 | 步骤10-Security挂载最小集-完整实施指南.md | `step10-security-min` | `learn.spring.step10` | 8210 |
| 11 | 步骤11-测试与Testcontainers-完整实施指南.md | `step11-testing-testcontainers` | `learn.spring.step11` | 8211 |
| 12 | 步骤12-Actuator与日志入口-完整实施指南.md | `step12-actuator-logging` | `learn.spring.step12` | 8212 |
| 13 | 第3部分-数据层深化-完整实施指南.md | `part03-data-layer` | `learn.spring.part03` | 8213 |
| 14 | 第4部分-安全深化-完整实施指南.md | `part04-security-deep` | `learn.spring.part04` | 8214 |
| 15 | 第5部分-API设计与模块化单体-完整实施指南.md | `part05-api-modulith` | `learn.spring.part05` | 8215 |
| 16 | 第6部分-弹性与失败处理-完整实施指南.md | `part06-resilience` | `learn.spring.part06` | 8216 |
| 17 | 第7部分-消息与异步-完整实施指南.md | `part07-messaging-async` | `learn.spring.part07` | 8217 |
| 18 | 第8部分-分布式与SpringCloud-完整实施指南.md | `part08-distributed-cloud` | `learn.spring.part08` | 8218 |
| 19 | 第9部分-可观测与排障-完整实施指南.md | `part09-observability` | `learn.spring.part09` | 8219 |
| 20 | 第10部分-部署与交付-完整实施指南.md | `part10-deploy-delivery` | `learn.spring.part10` | 8220 |
| 21 | 第11部分-性能与AOT与Native与源码-完整实施指南.md | `part11-performance-aot-native` | `learn.spring.part11` | 8221 |
| 22 | 第12部分-选学支线-完整六要素摘要.md | `part12-electives` | `learn.spring.part12` | 8222 |
| 23 | 第13部分-Capstone1-生产级单体REST-API-完整实施指南.md | `capstone01-rest-api` | `learn.spring.cs01` | 8223 |
| 24 | 第14部分-Capstone2-模块化单体Modulith-完整实施指南.md | `capstone02-modulith` | `learn.spring.cs02` | 8224 |
| 25 | 第15部分-Capstone3-云原生多服务-完整实施指南.md | `capstone03-cloud-native` | `learn.spring.cs03` | 8225 |

**Excluded:** `Spring-Boot4-Spring-Framework7-学习路线图.md` (meta roadmap, not a lab chapter).

## File map

| Path | Responsibility |
|---|---|
| `settings.gradle.kts` | Dynamic include `:spring` + `:spring:<lab>` from `src/spring/*` |
| `build.gradle.kts` | Plugins apply false + `assembleSpringBoot` aggregate |
| `gradle/libs.versions.toml` | Optional pin for boot version (or hardcode 4.1.0 like AGP) |
| `src/spring/build.gradle.kts` | Empty container (like `src/android`) |
| `src/spring/README.md` | Lab index / how to run |
| `src/spring/<lab>/build.gradle.kts` | Boot app module |
| `src/spring/<lab>/src/main/java/.../Application.java` | `@SpringBootApplication` |
| `src/spring/<lab>/src/main/java/.../HelloController.java` | `GET /` HelloWorld |
| `src/spring/<lab>/src/main/resources/application.properties` | name + unique port |
| `src/spring/<lab>/README.md` | Doc link + package + port |
| `.run/Spring__<lab>.run.xml` | Gradle `:spring:<lab>:bootRun` |
| `.run/Spring__ALL_assembleSpringBoot.run.xml` | Aggregate task |
| `.github/workflows/{linux,macos,windows}-ci.yml` | After JVM (or Android): `assembleSpringBoot` |

## Per-module template (identical shape)

**build.gradle.kts:**
- plugins: `java`, `org.springframework.boot`, `io.spring.dependency-management`
- Java toolchain 21
- deps: `spring-boot-starter-web` + test starter
- group `learn.spring`, version `0.1.0-placeholder`

**Application.java:** standard main + SpringApplication.run
**HelloController.java:** REST `GET /` returns JSON message with lab title
**application.properties:** `spring.application.name`, `server.port`

## Root Gradle wiring

```kotlin
// settings.gradle.kts — after android block
include(":spring")
project(":spring").projectDir = file("src/spring")
// filter dirs with build.gradle.kts + src/main/java

// build.gradle.kts
id("org.springframework.boot") version "4.1.0" apply false
id("io.spring.dependency-management") version "1.1.7" apply false

tasks.register("assembleSpringBoot") {
  dependsOn(provider {
    subprojects.filter { it.path.startsWith(":spring:") && it.path != ":spring" }
      .map { "${it.path}:bootJar" }
  })
}
```

## CI

Same three workflows, after Android assemble (or after LearnJava if Android fails on some runners — prefer after Java smoke, can be after Android):

```yaml
- name: Assemble Spring Boot labs (bootJar)
  run: ./gradlew assembleSpringBoot --stacktrace --no-daemon
```

No extra SDK — only existing JDK 25 + Gradle. Windows: only `build` job (not slnx), same as Android pattern.

## Verification

```bat
gradlew.bat assembleSpringBoot --no-daemon
```

Expected: BUILD SUCCESSFUL; 25 `bootJar` outputs.

Optional smoke one module:
```bat
gradlew.bat :spring:step01-startup-model:bootRun
```
(not required in CI)

## Commit / push

1. Single commit (or two: scaffold then CI) — prefer one coherent commit if clean.
2. `git push -u origin feature/spring-boot-labs`
3. **Do not** `gh pr create`

## Self-review

1. All 25 guides covered; roadmap excluded.
2. Android parity: multi-project, `.run`, aggregate task, CI, README.
3. Ports unique; packages unique; no default-package.
4. HelloWorld only — no premature Security/JPA deps that break compile.
5. No PR per user request.
